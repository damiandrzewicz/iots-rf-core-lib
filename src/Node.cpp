#if defined(__AVR__)

#include <Arduino.h>
#include <ArduinoLog.h>
#include "Node.hpp"

const char* const stateName[] PROGMEM = { 
        "VerifyConfig",
        "SleepUntilExtInt",
        "CheckStateBtn",
        "RadioPairing",
        "RadioReset",
        "FactoryReset",
        "MessageBuild",
        "MessageHandle",
        "SendToRadio",
        "ListenFromRadio",
        "SleepForOrUntilExtInt"
};

void Node::setupStateMachine()
{
    Log.verboseln(F("Node::setupStateMachine"));

    /** States */

    stateMachine_.AddState(stateName[static_cast<int>(State::VerifyConfig)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onVerifyConfig(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::SleepUntilExtInt)], 50,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onSleepUntilExtInt(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::CheckStateBtn)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterCheckStateBtn(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onCheckStateBtn(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onCheckStateBtn(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::RadioPairing)], 5000,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onRadioPairing(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::RadioReset)], 3000,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onRadioReset(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::FactoryReset)], 3000,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onFactoryReset(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::MessageBuild)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onMessageBuild(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::MessageHandle)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onMessageBuild(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::SendToRadio)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onSendToRadio(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::ListenFromRadio)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onListenFromRadio(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::SleepForOrUntilExtInt)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onSleepForOrUntilExtInt(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );



    /** Transitions */

    stateMachine_.AddTransition(static_cast<int>(State::VerifyConfig), static_cast<int>(State::SleepUntilExtInt), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Failed;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::SleepUntilExtInt), static_cast<int>(State::CheckStateBtn), 
        [](void *ctx){
            return
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::CheckStateBtn), static_cast<int>(State::RadioPairing), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout &&
                static_cast<Node*>(ctx)->stateBtnMode_ == StateBtnMode::Pairing;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::CheckStateBtn), static_cast<int>(State::RadioReset), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout &&
                static_cast<Node*>(ctx)->stateBtnMode_ == StateBtnMode::RadioReset;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::CheckStateBtn), static_cast<int>(State::FactoryReset), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout &&
                static_cast<Node*>(ctx)->stateBtnMode_ == StateBtnMode::FactoryReset;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::CheckStateBtn), static_cast<int>(State::VerifyConfig), 
        [](void *ctx){
            return static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout &&
                 static_cast<Node*>(ctx)->stateBtn_.getState() == HIGH;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioPairing), static_cast<int>(State::SleepUntilExtInt), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Failed;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioPairing), static_cast<int>(State::VerifyConfig), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioReset), static_cast<int>(State::VerifyConfig), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout;    
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::FactoryReset), static_cast<int>(State::VerifyConfig), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout;   
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::VerifyConfig), static_cast<int>(State::MessageBuild), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::MessageBuild), static_cast<int>(State::SendToRadio), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::MessageBuild), static_cast<int>(State::SleepForOrUntilExtInt), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Failed;      
        },
        this    
    );


    stateMachine_.AddTransition(static_cast<int>(State::SendToRadio), static_cast<int>(State::ListenFromRadio), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::SendToRadio), static_cast<int>(State::SleepForOrUntilExtInt), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Failed;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::ListenFromRadio), static_cast<int>(State::MessageHandle), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::MessageHandle), static_cast<int>(State::SleepForOrUntilExtInt), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::ListenFromRadio), static_cast<int>(State::SleepForOrUntilExtInt), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Failed;      
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::SleepForOrUntilExtInt), static_cast<int>(State::CheckStateBtn), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;      
        },
        this    
    );
}

void Node::init()
{
    Log.verboseln(F("Node::init"));

    radioConfig_.clear();
    radioConfig_.save();

    uuidConfig_.clear();
    uuidConfig_.save();


    if(uuidConfig_.isEmpty())
    {
        // Build new UUID value and save it to EEPROM
    }
}

void Node::stateBtnLoop()
{
    Appliance::stateBtnLoop();
    stateBtnMode_ = checkStateBtn();
}

/**
 * States
 * 
 */

// VerifyConfig,
void Node::onVerifyConfig()
{
    Log.verboseln(F("Node::onVerifyConfig"));

    if(radioConfig_.isEmpty())
    {
        stateMachine_.CurrentState()->Result = FSM_State::Result::Failed;
    }
    else
    {
        stateMachine_.CurrentState()->Result = FSM_State::Result::Success;

        // setupRadio
    }

    stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
}

// SleepUntilExtInt,
void Node::onSleepUntilExtInt()
{
    Log.verboseln(F("Node::onSleepUntilExtInt"));
    deepSleepForWakeupOnInt(0, extInterruptPin_, FALLING);
    stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
}

// StateBtnHandle,
void Node::onEnterCheckStateBtn()
{
    onEnterActiveStateName();
    stateMachine_.CurrentState()->maxTime = stateBtn_.getDebounceTime() + 5;
    stateLed_.blink(50, 950, 1000);
}

void Node::onCheckStateBtn()
{

}

void Node::onLeaveCheckStateBtn()
{
    onLeaveActiveStateName();
    stateLed_.cancel();
}

// RadioPairing
void Node::onRadioPairing()
{
    auto currentState = stateMachine_.CurrentState();
    auto success = true;

    if(!currentState->timeout)
    {
        if(currentState->step == 1)
        {
            stateLed_.blink(50, 250);
            currentState->step++;
        }

        static uint32_t execTime = 0;
        if(millis() - execTime > 1000L){
            execTime = millis();
            Log.verboseln(F("do some stuff..."));
        }
    }
    else if(currentState->step == 2)
    {   
        stateLed_.cancel();
        success ? stateLed_.blinkInPeriod(1000, 0, 3000) : stateLed_.blinkNumberOfTimes(500, 500, 5);
        currentState->step++;
    }
    else if(currentState->step == 3)
    {
        // Led finished work
        if(stateLed_.getState() == LED_IDLE)
        {
            Log.verboseln(F("Led finished work"));
            currentState->Result = success ? FSM_State::Result::Success : FSM_State::Result::Failed;
        }
    }
}

// RadioReset,
void Node::onRadioReset()
{
    // TODO radioConfig reset

}

// FactoryReset
void Node::onFactoryReset()
{

}

// MessageBuild,
void Node::onSendToRadio()
{
    stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
}

// SendToRadio,
void Node::onMessageBuild()
{
    stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
}

// MessageHandle
void Node::onMessageHandle()
{
    stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
}

// ListenFromRadio,
void Node::onListenFromRadio()
{
    stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
}

// SleepForOrUntilExtInt
void Node::onSleepForOrUntilExtInt()
{
    stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
   deepSleepForWakeupOnInt(5000, extInterruptPin_, FALLING);
}

// void Node::onPairing()
// {
//     auto currentState = stateMachine_.CurrentState();
//     auto success = true;

//     if(!currentState->timeout)
//     {
//         if(currentState->step == 1)
//         {
//             stateLed_.blink(150, 150);
//             currentState->step++;
//         }

//         static uint32_t execTime = 0;
//         if(millis() - execTime > 1000L){
//             execTime = millis();
//             Log.verboseln(F("do some stuff..."));
//         }
//     }
//     else if(currentState->step == 2)
//     {   
//         stateLed_.cancel();
//         success ? stateLed_.blinkInPeriod(1000, 0, 3000) : stateLed_.blinkNumberOfTimes(500, 500, 5);
//         currentState->step++;
//     }
//     else if(currentState->step == 3)
//     {
//         // Led finished work
//         if(stateLed_.getState() == LED_IDLE)
//         {
//             Log.verboseln(F("Led finished work"));
//             currentState->Result = success ? FSM_State::Result::Success : FSM_State::Result::Failed;
//         }
//     }
// }

// void Node::onRadioListen()
// {
//     //Log.verboseln(F("Node::onRadioListen"));
    
// }

// void Node::onSendState()
// {
//     Log.verboseln(F("Node::onSendState"));
//     stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
// }

// void Node::onSleep()
// {
//     Log.verboseln(F("Node::onSleep"));

//     deepSleepForWakeupOnInt(5000, 3, FALLING);
//     stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
// }

Node::StateBtnMode Node::checkStateBtn()
{
    static constexpr uint16_t pairingPressTime = 3000;
    static constexpr uint16_t radioResetPressTime = 10000;
    static constexpr uint16_t factoryResetPressTime = 15000;

    static unsigned long pressedTime = 0;
    static unsigned long releasedTime = 0;

    StateBtnMode mode = StateBtnMode::NoMode;

    if(stateBtn_.isPressed()){
        pressedTime = millis();
        //Log.verboseln(F("pressed: %d"), pressedTime);
    }
    
    if(stateBtn_.isReleased()) {
        releasedTime = millis();
        //Log.verboseln(F("released: %d"), releasedTime);

        long pressDuration = releasedTime - pressedTime;

        if( pressDuration >= pairingPressTime ){
            mode = StateBtnMode::Pairing;
        }

        if( (pressDuration >= radioResetPressTime) && (pressDuration < factoryResetPressTime) ){
            mode = StateBtnMode::RadioReset;
        }

        if( pressDuration >= factoryResetPressTime ){
            mode = StateBtnMode::FactoryReset;
        }
    }
     //Log.verboseln(F("btnMode: %d, pressedTime: %d, releasedTime: %d"), 
     //   static_cast<uint8_t>(mode), pressedTime, releasedTime);

    return mode;
}

#endif