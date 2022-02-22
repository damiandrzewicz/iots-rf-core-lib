#pragma once 

#if defined(__AVR__)

    #include <LowPower.h>
    #include <Arduino.h>

    class LowPowerWrapper
    {
    public:
        void DeepSleep(uint32_t sleepTime);
        void DeepSleepForever();
    };

    extern LowPowerWrapper LowPowerWrp;
#endif