#pragma once

#if defined(__AVR__)

#include <Arduino.h>
#include "Appliance.hpp"
#include "UUIDConfig.hpp"

class Node : public Appliance
{
public:
    Node() : Appliance(4, 9, 3), uuidConfig_(radioConfig_.dataSize() + 1){}

protected:

    enum class State
    {
        VerifyConfig,
        SleepUntilExtInt,
        StateBtnHandle,
        RadioPairing,
        RadioReset,
        FactoryReset
    };

    enum class StateBtnMode
    {
        NoMode,
        Pairing,
        RadioReset,
        FactoryReset
    };

    void setupStateMachine() override;
    void init() override;
    
    virtual void stateBtnLoop() override;

    /**
     * States
     * 
     */

    // VerifyConfig,
    void onVerifyConfig();

    // SleepUntilExtInt,
    void onSleepUntilExtInt();

    // StateBtnHandle,
    void onEnterStateBtnHandle();
    void onStateBtnHandle();
    void onLeaveStateBtnHandle();

    // RadioPairing
    void onRadioPairing();

    // RadioReset,
    void onRadioReset();

    // FactoryReset
    void onFactoryReset();


    // Inputs
    StateBtnMode checkStateBtn();

    // Helpers

protected:
    UUIDConfig uuidConfig_;
    StateBtnMode stateBtnMode_ = StateBtnMode::NoMode;
};

#endif