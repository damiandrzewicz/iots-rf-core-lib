#include "Appliance.hpp"
#include <ArduinoLog.h>
#include <EEPROM.h>

#if defined(__AVR__)
    #include "utils/LowPowerWrapper.h"
#endif

Appliance::Appliance(uint8_t stateBtnPin, uint8_t stateLedPin, int8_t extInterruptPin)
    : stateBtn_(stateBtnPin), stateLed_(stateLedPin), extInterruptPin_(extInterruptPin)
{
    Log.verboseln(F("Appliance::Appliance"));
    Log.noticeln(F("stateBtnPin: %d, stateLedPin: %d, extInterruptPin: %d"), stateBtnPin, stateLedPin, extInterruptPin);

    radioConfigPairing_ = getRadioConfigForPairing();
}

Appliance::Appliance(uint8_t nssPin, uint8_t irqPin, uint8_t stateBtnPin, uint8_t stateLedPin, int8_t extInterruptPin)
    : Appliance(stateBtnPin, stateLedPin, extInterruptPin)
{
    radio_ = { nssPin, irqPin };
}

void Appliance::setup()
{
    Log.verboseln(F("Appliance::setup"));

    stateBtn_.setDebounceTime(40);

    EEPROM.get(0, radioConfig_);
    // if(radioConfig_.isEmpty())
    // {
    //     radioConfig_ = radioConfig_.getDefaults();
    //     radioSetup(255, radioConfig_);   // Setup radio with defaults, it's necessary for radio sleep properly
    // }

    init();
    setupStateMachine();

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
    stateBtnMode_ = checkStateBtn();
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
    
    if(radioInitialized_)
    {
        radio_.sleep();
    }
}

void Appliance::postDeepSleep()
{
    Log.verboseln(F("Appliance::postDeepSleep"));
}   

#endif

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
bool Appliance::radioSetup(uint8_t nodeId, const RadioConfig &radioConfig)
{
    Log.verboseln(F("Appliance::radioSetup"));

    Log.noticeln(F("RadioConfig(nodeId=%d, networkId=%d, rssi=%d, customFreq=%u, encryptKey=%s, gatewayId=%d"), 
        nodeId, radioConfig.networkId, radioConfig.rssi, radioConfig.customFrequency, radioConfig.encryptKey, radioConfig.gatewayId);

    if(radio_.initialize(RF69_868MHZ, nodeId, radioConfig.networkId))
    {
        if(radioConfig.customFrequency)
        {
            radio_.setFrequency(radioConfig.customFrequency);
        }

        radio_.setHighPower();
        radio_.enableAutoPower(radioConfig.rssi);
        radio_.encrypt(radioConfig.encryptKey);
        radio_.sleep();

        Log.noticeln(F("setup ok!"));

        radioInitialized_ = true;

        return true;
    }
    else
    {
        Log.fatalln(F("setup failed!"));
        return false;
    }
}

bool Appliance::radioSend(uint8_t gatewayId, const MessageBuffer *request, MessageBuffer *response, bool ack)
{
    Log.verboseln(F("Appliance::radioSend"));

    auto data = request->buffer()->data();
    auto size = strlen(data);

    Log.noticeln(F("[RADIO OUT<<<<]: data=%s, size=%d, ack=%d, gatewayId=%d"), data, size, ack, gatewayId);

    if(ack)
    {
        if(radio_.sendWithRetry(gatewayId, data, size, 2, 100U))
        {
            Log.noticeln(F("send ok!"));
            if(radioPayloadToBuffer())
            {
                Log.noticeln(F("got payload into buffer!"));
                response = &messageBuffer_;
                return true;
            }
            else
            {
                Log.warningln(F("missing response!"));
                return false;
            }
        }
        else
        {
            Log.warningln(F("radio sent FAILED!"));
            return false;
        }
    }
    else
    {
        radio_.send(gatewayId, data, size);
        return true;
    }
}

bool Appliance::radioPayloadToBuffer()
{
    //Log.verboseln(F("Appliance::radioPayloadToBuffer"));

    if (radio_.DATALEN) 
    {
        auto data = reinterpret_cast<const char*>(radio_.DATA);
        auto size = strlen(data);
        
        if(radio_.DATALEN == size)  // got a valid packet?
        {
            messageBuffer_ = data;  // copy message to the buffer
            Log.noticeln(F("[RADIO IN>>>>]: data=%s, size=%d, senderId=%d, rssi=%d"), messageBuffer_.buffer()->data(), size, radio_.SENDERID, radio_.RSSI);
            return true;
        }
    }

    return false;
}

void Appliance::sendACKRepsonse(const MessageBuffer *request)
{
    //Log.verboseln(F("Appliance::sendACKRepsonse"));
    // Do not add logs before send ACK because of timing issues!

    if(request)
    {
        auto data = request->buffer()->data();
        auto size = strlen(data);
        radio_.sendACK(data, size);
        Log.noticeln(F("[RADIO OUT(AKC)<<<<]: data=%s, size=%d, senderId=%d"), data, size, radio_.SENDERID);
    }
    else
    {
        radio_.sendACK();
        Log.noticeln(F("[RADIO OUT(AKC)<<<<]: EMPTY, senderId=%d"),  radio_.SENDERID);
    }
}

RadioConfig Appliance::getRadioConfigForPairing()
{
    Log.verboseln(F("Appliance::getRadioConfigForPairing"));

    RadioConfig radioConfigData;
    radioConfigData.gatewayId = 1;
    radioConfigData.networkId = 111;
    radioConfigData.customFrequency = 869000000L;
    strcpy_P(radioConfigData.encryptKey, PSTR("sampleEncryptKey"));
    radioConfigData.rssi = -80;

    return radioConfigData;
}

// Inputs
Appliance::StateBtnMode Appliance::checkStateBtn()
{
    static constexpr uint16_t pairingPressTime = 3000;
    static constexpr uint16_t radioResetPressTime = 10000;
    static constexpr uint16_t factoryResetPressTime = 15000;

    static unsigned long pressedTime = 0;
    static unsigned long releasedTime = 0;

    StateBtnMode mode = StateBtnMode::NoMode;

    if(stateBtn_.isPressed()){
        pressedTime = millis();
        //Log.verboseln(F("pressed: %d"), pressedTime);
    }
    
    if(stateBtn_.isReleased()) {
        releasedTime = millis();
        //Log.verboseln(F("released: %d"), releasedTime);

        long pressDuration = releasedTime - pressedTime;

        if( pressDuration >= pairingPressTime ){
            mode = StateBtnMode::Pairing;
        }

        if( (pressDuration >= radioResetPressTime) && (pressDuration < factoryResetPressTime) ){
            mode = StateBtnMode::RadioReset;
        }

        if( pressDuration >= factoryResetPressTime ){
            mode = StateBtnMode::FactoryReset;
        }
    }
     //Log.verboseln(F("btnMode: %d, pressedTime: %d, releasedTime: %d"), 
     //   static_cast<uint8_t>(mode), pressedTime, releasedTime);

    return mode;
}
