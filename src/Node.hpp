#pragma once

#if defined(__AVR__)

#include <Arduino.h>
#include "Appliance.hpp"
#include "UUIDConfig.hpp"

class Node : public Appliance
{
public:
    Node() : Appliance(4, 9, 3){}

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

    void setupStateMachine() override;
    void init() override;

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
    void onEnterRadioPairing();
    void onRadioPairing();
    void onLeaveRadioPairing();

    // FactoryReset,
    //void onFactoryReset();

    // RadioReset,
    void onRadioReset();



    // Helpers

protected:
    //UUIDConfig uuidConfig_;
    
};

#endif