#if defined(__AVR__)

#include <Arduino.h>
#include <ArduinoLog.h>
#include "Node.hpp"

const char* const stateName[] PROGMEM = { 
    "VerifyConfig",
    "RadioSend",
    "RadioListen",
    "SleepForOrUntilExtInt",
    "ActionHandler",
    "RadioPairing",
    "FactoryReset",
    "RadioReset"
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

    stateMachine_.AddState(stateName[static_cast<int>(State::RadioSend)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onRadioSend(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );
    
    stateMachine_.AddState(stateName[static_cast<int>(State::RadioListen)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActionHandler(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onRadioListen(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActionHandler(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::SleepForOrUntilExtInt)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onSleepForOrUntilExtInt(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::ActionHandler)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActionHandler(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onActionHandler(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActionHandler(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::RadioPairing)], 5000,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onRadioPairing(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::FactoryReset)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onFactoryReset(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::RadioReset)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onRadioReset(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    // /** Transitions */

    stateMachine_.AddTransition(static_cast<int>(State::VerifyConfig), static_cast<int>(State::RadioSend), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result == FSM_State::Result::Success;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::VerifyConfig), static_cast<int>(State::SleepForOrUntilExtInt), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result == FSM_State::Result::Failed;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioSend), static_cast<int>(State::RadioListen), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result == FSM_State::Result::Success;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioSend), static_cast<int>(State::SleepForOrUntilExtInt), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result == FSM_State::Result::Failed;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioListen), static_cast<int>(State::SleepForOrUntilExtInt), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result != FSM_State::Result::InWork;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::SleepForOrUntilExtInt), static_cast<int>(State::ActionHandler), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result != FSM_State::Result::InWork;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::ActionHandler), static_cast<int>(State::RadioPairing), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout &&
                static_cast<Node*>(ctx)->stateBtnMode_ == StateBtnMode::Pairing;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::ActionHandler), static_cast<int>(State::FactoryReset), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout &&
                static_cast<Node*>(ctx)->stateBtnMode_ == StateBtnMode::FactoryReset;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::ActionHandler), static_cast<int>(State::RadioReset), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout &&
                static_cast<Node*>(ctx)->stateBtnMode_ == StateBtnMode::RadioReset;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::ActionHandler), static_cast<int>(State::RadioSend), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout &&
                static_cast<Node*>(ctx)->stateBtn_.getState() == HIGH;
  
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioPairing), static_cast<int>(State::VerifyConfig), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout &&
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result != FSM_State::Result::InWork;
  
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
    if(radioConfig_.isEmpty())
    {
        stateMachine_.CurrentState()->result = FSM_State::Result::Failed;
    }
    else
    {
        stateMachine_.CurrentState()->result = FSM_State::Result::Success;

        // setupRadio
    }

    nextSleepTime_ = 0;
    stateMachine_.CurrentState()->result = FSM_State::Result::Failed;
}

// RadioSend,
void Node::onRadioSend()
{
    stateMachine_.CurrentState()->result = FSM_State::Result::Success;
}

// RadioListen,
void Node::onRadioListen()
{
    stateMachine_.CurrentState()->result = FSM_State::Result::Success;
}

// SleepForOrUntilExtInt
void Node::onSleepForOrUntilExtInt()
{
    Log.verboseln(F("Node::onSleepUntilExtInt"));
    deepSleepForWakeupOnInt(nextSleepTime_, extInterruptPin_, FALLING);
    stateMachine_.CurrentState()->result = FSM_State::Result::Success;
}

// ActionHandler,
void Node::onEnterActionHandler()
{
    onEnterActiveStateName();
    stateMachine_.CurrentState()->maxTime = stateBtn_.getDebounceTime() + 5;
    stateLed_.blink(50, 950, 1000);
}

void Node::onActionHandler()
{

}

void Node::onLeaveActionHandler()
{
    onLeaveActiveStateName();
    stateLed_.cancel();
}

// RadioPairing,
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
            currentState->result = success ? FSM_State::Result::Success : FSM_State::Result::Failed;
        }
    }
}

// FactoryReset,
// void Node::onFactoryReset()
// {
//     stateMachine_.CurrentState()->result = FSM_State::Result::Success;
// }

// RadioReset,
void Node::onRadioReset()
{
    stateMachine_.CurrentState()->result = FSM_State::Result::Success;
}

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