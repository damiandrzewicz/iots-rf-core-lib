#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "RFM69_ATC.h"
#include "YA_FSM.h"
//#include <arduino-timer.h>

#include "RadioConfig.hpp"
//#include "MessageBuffer.hpp"

class Appliance
{
public:
    Appliance(uint8_t radioPairBtn, uint8_t statusLed, int8_t extInterrupt = -1);
    ~Appliance() = default;

    virtual void setup();
    virtual void loop();

protected:

    virtual void setupStateMachine() = 0;
    virtual void init() = 0;

#if defined(__AVR__)
    // Power saving operations
    void deepSleepFor(unsigned int delay);
    void deepSleepForWakeupOnInt(unsigned int delay_ms, uint8_t pin, uint8_t mode);
    virtual void preDeepSleep();
    virtual void postDeepSleep();
#endif

    void onEnterActiveStateName();
    void onLeaveActiveStateName();

protected:
    YA_FSM stateMachine_;

    uint8_t radioPairBtn_;
    uint8_t statusLed_;
    int8_t extInterrupt_;

    RFM69_ATC radio_;
    RadioConfig radioConfig_;

};