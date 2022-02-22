#pragma once

#include <Arduino.h>

class StatusLed
{
public:
    StatusLed(uint8_t pin)
        : pin_(pin)
    {
        pinMode(pin_, OUTPUT);
    }

    ~StatusLed() = default;

    void startBlink(uint16_t blinkSpeed)
    {
        blinkSpeed_ = blinkSpeed;
    }

    void stopBlink()
    {
        blinkSpeed_ = -1;
        digitalWrite(pin_, LOW);
    }

    void execISR()
    {
        static int16_t tick = 0;
        if(blinkSpeed_ == -1 || (++tick < blinkSpeed_)) { return; }

        digitalWrite(pin_, !digitalRead(pin_));

        tick = 0;
    }
    
private:
    const uint8_t pin_;
    volatile int16_t blinkSpeed_ = -1;
};