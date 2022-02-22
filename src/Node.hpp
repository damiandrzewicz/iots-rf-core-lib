#pragma once

#if defined(__AVR__)

#include <Arduino.h>
#include "Appliance.hpp"
#include "UUIDConfig.hpp"
#include "StatusLed.hpp"


#define FLASH_SS 8

class Node : public Appliance
{
public:
    Node()
        :   Appliance(4),
            uuidConfig_(radioConfig_.dataSize() + 1)
    {}

    virtual void setup() override;
    virtual void loop() override;

protected:

    friend void timer1Handler();

    // Configuration
    virtual void readConfiguration();

    // Radio operations

    // void readConfiguration();

    // // Radio operations
    // bool isRadioConfigured();
    // bool isRadioPairing(); 


    // Status Led
    virtual void statusLedBlink(int16_t speed) override;

private:
    UUIDConfig uuidConfig_;
    static StatusLed statusLed;
};

#endif