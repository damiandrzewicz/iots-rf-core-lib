#pragma once

#include <Arduino.h>
#include "Config.hpp"
#include <EEPROM.h>

template<class T>
class EEPROMConfig : public Config
{
public:
    EEPROMConfig(int address) : address_(address){}
    virtual ~EEPROMConfig() = default;

    virtual void save() override {
        EEPROM.put(address_, data_);
    }

    virtual void read() override {
        EEPROM.get(address_, data_);
    }

    virtual void clear() override {
        setEmpty();
    }

    virtual bool isEmpty() = 0;
    virtual void setEmpty() = 0;
    virtual void setDefaults() = 0;

    T &data(){
        return data_;
    }

    size_t dataSize(){
        return sizeof(T);
    }

protected:
    int address_;
    T data_;
    uint8_t emptyVal_ = 0xff;
};