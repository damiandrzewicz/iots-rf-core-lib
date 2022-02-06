#pragma once

#include <Arduino.h>
#include <EEPROM.h>

template<class T>
class EEPROMConfig
{
public:
    EEPROMConfig(int address) : address_(address){}
    virtual ~EEPROMConfig() = default;

    void save(){
        EEPROM.put(address_, data_);
    }

    void read(){
        EEPROM.get(address_, data_);
    }

    void clear(){
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