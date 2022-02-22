#pragma once

#include <Arduino.h>
#include "Appliance.hpp"
#include "UUIDConfig.hpp"
#include "StatusLed.hpp"


#define FLASH_SS 8

class Node : public Appliance
{
public:
    Node()
        :   
            uuidConfig_(radioConfig_.dataSize() + 1)
    {}

    virtual void setup() override;
    virtual void loop() override;

protected:

    friend void timer1Handler();

    // Configuration
    virtual void readConfiguration();

    // Radio operations
    virtual bool isRadioPairTriggered() override;

    // void readConfiguration();

    // // Radio operations
    // bool isRadioConfigured();
    // bool isRadioPairing(); 


    // Status Led
    virtual void statusLedBlink(int16_t speed) override;

private:
    UUIDConfig uuidConfig_;
    static StatusLed statusLed;
    static const uint8_t s_radioPairPin_ = 4;
};