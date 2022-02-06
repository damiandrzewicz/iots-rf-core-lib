#pragma once

#include "EEPROMConfig.hpp"

struct RadioConfigData{
    uint8_t networkId;
    uint8_t gatewayId;
    unsigned long customFrequency;
    char encryptKey[17];
    int8_t powerLevel;
};

class RadioConfig : public EEPROMConfig<RadioConfigData>
{
public:
    RadioConfig(int address) : EEPROMConfig(address) {}

    virtual bool isEmpty() override{
        return data_.networkId == emptyVal_;
    }

    virtual void setEmpty() override {
        data_.networkId = emptyVal_;
    }

    virtual void setDefaults() override {
        data_.gatewayId = 1;
        data_.networkId = 100;
        data_.customFrequency = 0;
        data_.powerLevel = 0;
        strcpy_P(data_.encryptKey, PSTR("sampleEncryptKey"));
    }
};