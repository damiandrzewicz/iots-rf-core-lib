#include "Appliance.hpp"
#include <ArduinoLog.h>

#if defined(__AVR__)
    #include "utils/LowPowerWrapper.h"
#endif

Appliance::Appliance(uint8_t stateBtnPin, uint8_t stateLedPin, int8_t extInterruptPin)
    : stateBtn_(stateBtnPin), stateLed_(stateLedPin), extInterruptPin_(extInterruptPin), radioConfig_(0)
{
    Log.verboseln(F("Appliance::Appliance"));
    Log.noticeln(F("stateBtnPin: %d, stateLedPin: %d, extInterruptPin: %d"), stateBtnPin, stateLedPin, extInterruptPin);
}

void Appliance::setup()
{
    Log.verboseln(F("Appliance::setup"));

    init();
    setupStateMachine();

    stateBtn_.setDebounceTime(40);

    Log.noticeln(F("Active state: %s"), stateMachine_.ActiveStateName());
}

void Appliance::loop()
{
    //Log.verboseln(F("Appliance::loop"));

    stateBtnLoop();
    stateLedLoop();

    static uint8_t prevLedState = LED_IDLE;
    if(prevLedState != stateLed_.getState())
    {
        prevLedState = stateLed_.getState();
        Log.verboseln(F("led state changed: %d"), prevLedState);
    }

    if(stateMachine_.Update()){
        Log.noticeln(F("Active state: %s"), stateMachine_.ActiveStateName());
    }
}

void Appliance::setupStateMachine()
{
    Log.verboseln(F("Appliance::setupStateMachine"));
}

void Appliance::stateLedLoop()
{
    stateLed_.loop();
}

void Appliance::stateBtnLoop()
{
    stateBtn_.loop();
}

#if defined(__AVR__)


void Appliance::deepSleepFor(unsigned int delay_ms)
{
    Log.verboseln(F("Appliance::deepSleepFor for %d ms"), delay_ms);

    preDeepSleep();
    LowPowerWrp.DeepSleep(delay_ms);
    postDeepSleep();
}

void Appliance::deepSleepForWakeupOnInt(unsigned int delay, uint8_t pin, uint8_t mode)
{
    Log.verboseln(F("Appliance::deepSleepForWakeupOnInt for: %d, interrput(pin: %d, mode: %d)"), delay, pin, mode);
    
    preDeepSleep();
    attachInterrupt(digitalPinToInterrupt(pin), [](){}, mode);
    
    if(delay)
    {
        LowPowerWrp.DeepSleep(delay);
    }
    else
    {
        LowPowerWrp.DeepSleepForever();
    }

    detachInterrupt(digitalPinToInterrupt(pin)); 
    postDeepSleep();
}

void Appliance::preDeepSleep()
{
    Log.verboseln(F("Appliance::preDeepSleep"));

    Serial.flush();
    //radio_.sleep();
}

void Appliance::postDeepSleep()
{
    Log.verboseln(F("Appliance::postDeepSleep"));
}   

void Appliance::onEnterActiveStateName()
{
    Log.verboseln(F("onEnter: %s"), stateMachine_.ActiveStateName());
}

void Appliance::onLeaveActiveStateName()
{
    Log.verboseln(F("onLeave: %s (ret: %s, timeout: %d)"), 
        stateMachine_.ActiveStateName(), 
        stateMachine_.CurrentState()->result == FSM_State::Result::Success ? "success" 
            : stateMachine_.CurrentState()->result == FSM_State::Result::Failed ? "failed" : "inWork",
        stateMachine_.CurrentState()->timeout
    );
}

void Appliance::onFactoryReset()
{
    Log.verboseln(F("Appliance::onFactoryReset"));

    stateMachine_.CurrentState()->result = FSM_State::Result::Success;
}

// Radio operations
bool Appliance::radioSetup(uint8_t nodeId, const RadioConfigData &data)
{
    Log.verboseln(F("Appliance::radioSetup"));

    if(radio_.initialize(RF69_868MHZ, nodeId, data.networkId))
    {
        Log.noticeln(F("set: nodeId: %d, networkId: %d, powerLvl: %d, customFreq: %d, encryptKey: %s, gatewayId: %d"), 
            nodeId, data.networkId, data.powerLevel, data.customFrequency, data.encryptKey, data.gatewayId);

        if(data.customFrequency)
        {
            radio_.setFrequency(data.customFrequency);
        }

        radio_.setHighPower();
        radio_.setPowerLevel(data.powerLevel);
        radio_.encrypt(data.encryptKey);

        radio_.sleep();

        Log.noticeln(F("radio started in speel mode!"));

        return true;
    }

    return false;
}

bool Appliance::radioSend(const MessageBuffer *request, MessageBuffer *response, bool ack)
{
    Log.verboseln(F("Appliance::radioSend"));

    const auto &radioConf = radioConfig_.data();

    if(ack)
    {
        auto ret = radio_.sendWithRetry(radioConf.gatewayId, request->buffer()->data(), request->buffer()->size());
        if(ret)
        {
            Log.noticeln(F("radio sent!"));
            if(radioPayloadToBuffer())
            {
                response = &messageBuffer_;
                return true;
            }
        }
        return false;
    }
    else
    {
        radio_.send(radioConf.gatewayId, request->buffer()->data(), request->buffer()->size());
        return true;
    }
}

bool Appliance::radioPayloadToBuffer()
{
    Log.verboseln(F("Appliance::radioPayloadToBuffer"));
    return false;
}

void Appliance::sendACKRepsonse(const MessageBuffer *request)
{
    Log.verboseln(F("Appliance::sendACKRepsonse"));

    if(radio_.ACKRequested()){
        if(request)
        {
            auto buffer = messageBuffer_.buffer();
            Log.noticeln(F("[OUT] radio ACK: data=[%s], size=[%d]"), buffer->data(), buffer->size());
            radio_.sendACK(buffer->data(), buffer->size());
        }
        else
        {
            Log.noticeln(F("[OUT] radio ACK empty"));
            radio_.sendACK();
        }
    }
}

RadioConfigData Appliance::getRadioConfigForPairing()
{
    Log.verboseln(F("Appliance::getRadioConfigForPairing"));

    RadioConfigData radioConfigData;
    radioConfigData.gatewayId = 1;
    radioConfigData.networkId = 111;
    radioConfigData.customFrequency = 869000000;
    strcpy_P(radioConfigData.encryptKey, PSTR("sampleEncryptKey"));
    radioConfigData.powerLevel = 0;

    return radioConfigData;
}

#endif
