#pragma once

#include <Arduino.h>

class EEPROMConfig
{
public:
    struct Data{
        virtual bool isEmpty() = 0;
        virtual void setEmpty() = 0;

        int emptyVal_ = 0xff;
    };

    EEPROMConfig(int address) : address_(address){}
    virtual ~EEPROMConfig() = default;

    virtual void save() = 0;
    virtual void read() = 0;
    virtual void clear() = 0; 

protected:
    int address_;
};