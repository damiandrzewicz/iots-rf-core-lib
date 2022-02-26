#pragma once

#if defined(__AVR__)

#include <Arduino.h>
#include "Appliance.hpp"
#include "UUIDConfig.hpp"

class Node : public Appliance
{
public:
    Node() : Appliance(4,9), uuidConfig_(radioConfig_.dataSize() + 1){}

protected:

    enum class State
    {
        VerifyConfig,
        Pairing,
        RadioListen,
        SendState,
        Sleep
    };

    void setupStateMachine() override;
    void init() override;

    // States
    void onPairing();
    void onRadioListen();
    void onSendState();
    void onSleep();

    // Inputs
    bool checkButton();

    // Helpers
    bool statusLedBlink(int delay, int execFor = 0);

protected:
    UUIDConfig uuidConfig_;
};

#endif