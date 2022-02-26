#include "Appliance.hpp"
#include <ArduinoLog.h>

#if defined(__AVR__)
    #include "utils/LowPowerWrapper.h"
#endif

Appliance::Appliance(uint8_t radioPairBtn, uint8_t statusLed, int8_t extInterrupt)
    : radioPairBtn_(radioPairBtn), statusLed_(statusLed), extInterrupt_(extInterrupt), radioConfig_(0)
{
    Log.verboseln(F("Appliance::Appliance"));
}

void Appliance::setup()
{
    init();
    setupStateMachine();

    Log.noticeln(F("Active state: %s"), stateMachine_.ActiveStateName());
}

void Appliance::loop()
{
    if(stateMachine_.Update()){
        Log.noticeln(F("Active state: %s"), stateMachine_.ActiveStateName());
    }
}

void Appliance::setupStateMachine()
{
    Log.verboseln(F("Appliance::setupStateMachine"));
}

#if defined(__AVR__)


void Appliance::deepSleepFor(unsigned int delay_ms)
{
    Log.verboseln(F("Appliance::deepSleepFor for %d ms"), delay_ms);

    preDeepSleep();
    LowPowerWrp.DeepSleep(delay_ms);
    postDeepSleep();
}

void Appliance::deepSleepForWakeupOnInt(unsigned int delay_ms, uint8_t pin, uint8_t mode)
{
    Log.verboseln(F("Appliance::deepSleepForWakeupOnInt for: %d, interrput(pin: %d, mode: %d)"), pin, mode);
    
    preDeepSleep();
    attachInterrupt(digitalPinToInterrupt(pin), [](){}, mode);
    
    if(delay_ms)
    {
        LowPowerWrp.DeepSleep(delay_ms);
    }
    else
    {
        LowPowerWrp.DeepSleepForever();
    }

    detachInterrupt(0); 
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
    Log.verboseln(F("onLeave: %s"), stateMachine_.ActiveStateName());
}

#endif
