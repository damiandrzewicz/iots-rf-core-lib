#pragma once

#include "EEPROMConfig.hpp"
#include <EEPROM.h>

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

    virtual void save() override {
        EEPROM.put(address_, data_);
    };

    virtual void read() override {
        EEPROM.get(address_, data_);
        if(data_.isEmpty()) {
            // EEPROM was empty -> initialize default
            data_.gatewayId = 1;
            data_.networkId = 100;
            data_.customFrequency = 0;
            data_.powerLevel = 0;
            strcpy_P(data_.encryptKey, PSTR("sampleEncryptKey"));
        }
    };

    virtual void clear() override {
        for (unsigned int i = address_; i < address_ + sizeof(data_); i++)
        {
            EEPROM.write(i, 0xff);
        }
    };

private:
    RadioConfigData data_;
};