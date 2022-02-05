#pragma once


#include "EEPROMConfig.hpp"

class RadioConfig : public EEPROMConfig
{
public:
    struct RadioConfigData : public Data{
        virtual bool isEmpty() override { return networkId == emptyVal_; };
        virtual void setEmpty() override { networkId = emptyVal_; };

        uint8_t networkId = emptyVal_;
        uint8_t gatewayId;
        unsigned long customFrequency;
        char encryptKey[17];
        int8_t powerLevel;
    };

    RadioConfig(int address) : EEPROMConfig(address) {}

    virtual void save() override {};
    virtual void read() override {};
    virtual void clear() override {};
};