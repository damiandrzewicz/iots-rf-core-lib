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
        CheckStateBtn,
        RadioPairing,
        RadioReset,
        FactoryReset,
        MessageBuild,
        MessageHandle,
        SendToRadio,
        ListenFromRadio,
        SleepForOrUntilExtInt
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

    // CheckStateBtn,
    void onEnterCheckStateBtn();
    void onCheckStateBtn();
    void onLeaveCheckStateBtn();

    // RadioPairing
    void onRadioPairing();

    // RadioReset,
    void onRadioReset();

    // FactoryReset
    void onFactoryReset();

    // MessageBuild,
    void onMessageBuild();

    // MessageHandle
    void onMessageHandle();

    // SendToRadio,
    void onSendToRadio();

    // ListenFromRadio,
    void onListenFromRadio();

    // SleepForOrUntilExtInt
    void onSleepForOrUntilExtInt();


    // Inputs
    StateBtnMode checkStateBtn();

    // Helpers

protected:
    UUIDConfig uuidConfig_;
    StateBtnMode stateBtnMode_ = StateBtnMode::NoMode;
};

#endif