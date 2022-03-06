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
        RadioSend,
        RadioListen,
        SleepForOrUntilExtInt,
        ActionHandler,
        RadioPairing,
        FactoryReset,
        RadioReset
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

    // RadioSend,
    void onRadioSend();

    // RadioListen,
    void onRadioListen();

    // SleepForOrUntilExtInt
    void onSleepForOrUntilExtInt();

    // ActionHandler,
    void onEnterActionHandler();
    void onActionHandler();
    void onLeaveActionHandler();

    // RadioPairing,
    void onRadioPairing();

    // FactoryReset,
    //void onFactoryReset();

    // RadioReset,
    void onRadioReset();

    // Inputs
    StateBtnMode checkStateBtn();

    // Helpers

protected:
    UUIDConfig uuidConfig_;
    StateBtnMode stateBtnMode_ = StateBtnMode::NoMode;
};

#endif