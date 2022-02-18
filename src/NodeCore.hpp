#pragma once

#include <Arduino.h>
#include "Core.hpp"
#include <SPI.h>
#include "RFM69_ATC.h"
#include "RadioConfig.hpp"
#include "UUIDConfig.hpp"

#define FLASH_SS 8

extern "C" void TIMER1_COMPA_vect(void) __attribute__ ((signal));

class NodeCore : public Core
{
public:
    enum class LedMode{
        Pair,
        Success,
        Error
    };

    NodeCore()
        :   radioConfig_(0),
            uuidConfig_(radioConfig_.dataSize() + 1)
    {}

    virtual void setup() override;
    virtual void loop() override;

protected:
    void readConfiguration();

    // Radio operations
    bool isRadioConfigured();
    bool isRadioPairing();
    bool radioPairWorker();
    void setupRadio(const RadioConfigData &data);

    // Node operations
    bool registerNode();

    // Sleep functions
    void deepSleepDelay(unsigned int delay);
    void deepSleepForewerAndWakeInt(uint8_t pin, uint8_t mode);

    // Led timer

    void setLedMode(LedMode mode);
    void setupLedTimer();
    static void enableLedTimer();
    static void disableLedTimer();

    // Friends
    friend void TIMER1_COMPA_vect(void);


private:
    RFM69_ATC radio_;
    RadioConfig radioConfig_;
    UUIDConfig uuidConfig_;

    static const uint8_t s_radioPairPin_ = 4;
    static const uint8_t s_ledPin = 9;
    static uint8_t s_ledBlinkSpeed;
};