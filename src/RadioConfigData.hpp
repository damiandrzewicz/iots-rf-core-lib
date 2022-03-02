#pragma once

#include <Arduino.h>

struct RadioConfigData{
    uint8_t networkId;
    uint8_t gatewayId;
    unsigned long customFrequency = 0;
    char encryptKey[17];
    int8_t powerLevel = 0;
};