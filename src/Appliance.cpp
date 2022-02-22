#include "Appliance.hpp"
#include <ArduinoLog.h>
#include "utils/LowPowerWrapper.h"

Appliance::Appliance()
    : radioConfig_(0)
{
    Log.verboseln(F("Appliance::Appliance"));
}

void Appliance::readConfiguration()
{
    // Read all configuration from EEPROM or somewhere else
    Log.verboseln(F("Appliance::readConfiguration"));

    // Read Radio EEPROM config
    radioConfig_.read();
}

void Appliance::radioSetup(const RadioConfigData &data)
{
    Log.verboseln(F("Appliance::setupRadio"));
}

bool Appliance::radioSend(const MessageBuffer *request, MessageBuffer *response, bool ack = true)
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
    const char *data = reinterpret_cast<const char*>(radio_.DATA);
    if (radio_.DATALEN) 
    {
        if(radio_.DATALEN == strlen(data))  // got a valid packet?
        {
            messageBuffer_ = data;
            Log.noticeln(F("<--received: %s"), data);
            return true;
        }
        return false;
    }
    return false;
}

bool Appliance::radioPairRoutine()
{
    Log.verboseln(F("Appliance::radioPairRoutine IN"));

    radioSetup(getRadioConfigForPair());
    
    notifyPairStarted();

    bool pairResult = false;
    for(uint8_t i = 0; i < 8; ++i)
    {
        Log.noticeln(F("Pair attempt: %d"), i);

        // Build radio pair message

        // Send message and get response

        // Parse response and decide about result

        delay(1000);
    }
    
    // Led notifications
    pairResult ? notifyPairSuccess() : notifyPairFailed();

    return pairResult;
}

void Appliance::radioReceiveTask()
{
    if(radio_.receiveDone())
    {
        radioPayloadToBuffer();

        // Here process some incoming messages - TOTO in future
    }
}

RadioConfigData Appliance::getRadioConfigForPair()
{
    Log.verboseln(F("Appliance::getRadioConfigForPair"));

    RadioConfigData radioConfigData;
    radioConfigData.gatewayId = 1;
    radioConfigData.networkId = 111;
    radioConfigData.customFrequency = 869000000;
    strcpy_P(radioConfigData.encryptKey, PSTR("sampleEncryptKey"));
    radioConfigData.powerLevel = 0;

    return radioConfigData;
}

void Appliance::deepSleepDelay(unsigned int delay_ms)
{
    Log.verboseln(F("Appliance::deepSleepDelay for %d ms"), delay_ms);
    Serial.flush();
    //radio_.sleep();
    LowPowerWrp.DeepSleep(delay_ms);
}

void wakeUp()
{
    //DeviceCore::IsExtInterrupt = true;
}

void Appliance::deepSleepForewerAndWakeInt(uint8_t pin, uint8_t mode)
{
    Log.verboseln(F("Appliance::deepSleepForewerAndWakeInt, pin: %d, mode: %d"), pin, mode);
    Serial.flush();
    radio_.sleep();
    attachInterrupt(digitalPinToInterrupt(pin), wakeUp, mode);
    //IsExtInterrupt = false;    // Clear interrupt flag
    LowPowerWrp.DeepSleepForever();
    detachInterrupt(0); 
}

void Appliance::notifyPairStarted()
{
    Log.verboseln(F("Appliance::notifyPairStarted"));

    statusLedBlink(10);
}

void Appliance::notifyPairSuccess()
{
    Log.verboseln(F("Appliance::notifyPairSuccess"));

    statusLedBlink(0);
    delay(3000);
    statusLedBlink(-1);
}

void Appliance::notifyPairFailed()
{
    Log.verboseln(F("Appliance::notifyPairFailed"));

    statusLedBlink(30);
    delay(3000);
    statusLedBlink(-1);
}