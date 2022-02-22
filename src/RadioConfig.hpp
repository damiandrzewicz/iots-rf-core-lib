#pragma once

#include "RadioConfigData.hpp"
#include "EEPROMConfig.hpp"

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