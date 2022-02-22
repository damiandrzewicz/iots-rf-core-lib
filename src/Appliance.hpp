#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "RFM69_ATC.h"
#include "RadioConfig.hpp"
#include "MessageBuffer.hpp"

class Appliance
{
public:
    Appliance();
    ~Appliance() = default;

    virtual void setup() = 0;
    virtual void loop() = 0;

protected:
    // Configuration
    virtual void readConfiguration();

    // Radio operations
    void radioSetup(const RadioConfigData &data);
    bool radioSend(const MessageBuffer *request, MessageBuffer *response, bool ack = true);
    bool radioPayloadToBuffer();
    void radioReceiveTask();
    bool radioPairRoutine();
    virtual bool isRadioPairTriggered() = 0;
    RadioConfigData getRadioConfigForPair();

    // Power saving operations
    void deepSleepDelay(unsigned int delay);
    void deepSleepForewerAndWakeInt(uint8_t pin, uint8_t mode);

    // Notifications
    virtual void statusLedBlink(int16_t speed) = 0;
    virtual void notifyPairStarted();
    virtual void notifyPairSuccess();
    virtual void notifyPairFailed();

protected:
    RFM69_ATC radio_;
    RadioConfig radioConfig_;

    MessageBuffer messageBuffer_;
};