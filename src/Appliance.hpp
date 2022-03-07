#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "RFM69_ATC.h"
#include "YA_FSM.h"
#include <ezButton.h>
#include <ezLED.h>

#include "RadioConfig.hpp"
#include "MessageBuffer.hpp"

class Appliance
{
public:
    enum class StateBtnMode
    {
        NoMode,
        Pairing,
        RadioReset,
        FactoryReset
    };

    Appliance(uint8_t stateBtnPin, uint8_t stateLedPin, int8_t extInterruptPin = -1);
    Appliance(uint8_t nssPin, uint8_t irqPin, uint8_t stateBtnPin, uint8_t stateLedPin, int8_t extInterruptPin = -1);
    ~Appliance() = default;

    virtual void setup();
    virtual void loop();

protected:

    virtual void setupStateMachine() = 0;
    virtual void init() = 0;

    virtual void stateLedLoop();
    virtual void stateBtnLoop();

#if defined(__AVR__)
    // Power saving operations
    void deepSleepFor(unsigned int delay);
    void deepSleepForWakeupOnInt(unsigned int delay, uint8_t pin, uint8_t mode);
    virtual void preDeepSleep();
    virtual void postDeepSleep();
#endif

    void onEnterActiveStateName();
    void onLeaveActiveStateName();
    void onFactoryReset();

    // Radio operations
    bool radioSetup(uint8_t nodeId, const RadioConfig &radioConfig);
    bool radioSend(uint8_t gatewayId, const MessageBuffer *request, MessageBuffer *response, bool ack = true);
    bool radioPayloadToBuffer();
    void sendACKRepsonse(const MessageBuffer *request = nullptr);
    RadioConfig getRadioConfigForPairing();

    // Inputs
    StateBtnMode checkStateBtn();

private:
    //void 

protected:
    YA_FSM stateMachine_;

    ezButton stateBtn_;
    ezLED stateLed_;
    int8_t extInterruptPin_;
    unsigned long nextSleepTime_ = 0;

    RFM69_ATC radio_;
    RadioConfig radioConfig_;
    RadioConfig radioConfigPairing_;
    bool radioInitialized_ = false;

    MessageBuffer messageBuffer_;
    StateBtnMode stateBtnMode_ = StateBtnMode::NoMode;
};