#if defined(__AVR__)

#include <Arduino.h>
#include <ArduinoLog.h>
#include "Node.hpp"
#include "utils/UuidGenerator.hpp"

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
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterRadioPairing(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onRadioPairing(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveRadioPairing(); }, 
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

    stateMachine_.AddTransition(static_cast<int>(State::ActionHandler), static_cast<int>(State::VerifyConfig), 
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
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result != FSM_State::Result::InWork;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioReset), static_cast<int>(State::VerifyConfig), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result != FSM_State::Result::InWork;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::FactoryReset), static_cast<int>(State::VerifyConfig), 
        [](void *ctx){
            return 
                static_cast<Node*>(ctx)->stateMachine_.CurrentState()->result != FSM_State::Result::InWork;
        },
        this    
    );
}

void Node::init()
{
    Log.verboseln(F("Node::init"));

    UuidGenerator uuidGenerator;

    char uuid[50];
    memset(uuid, '\0', sizeof(uuid));
    uuidGenerator.generateUuid(uuid, sizeof(uuid));

    Log.noticeln(F("uuid: %s"), uuid);


    //uuidConfig_.clear();
    //uuidConfig_.save();

    //if(uuidConfig_.isEmpty())
    //{
        // Build new UUID value and save it to EEPROM
    //}
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
void Node::onEnterRadioPairing()
{
    onEnterActiveStateName();
    radioSetup(2, radioConfigPairing_);
}

void Node::onRadioPairing()
{
    auto currentState = stateMachine_.CurrentState();
    static auto success = false;

    if(currentState->step == 1)
    {
        stateLed_.blink(50, 250);
        currentState->step++;   // Next step
        success = false;
    }
    else if(currentState->step == 2)
    {
        if(!currentState->timeout)
        {
            // Keep pairing
            static uint32_t execTime = 0;
            if(millis() - execTime > 1000L){
                execTime = millis();
                
                // Prepare message
                messageBuffer_.clear();
                messageBuffer_.appendText("hello_123");

                MessageBuffer *response = nullptr;
                success = radioSend(radioConfigPairing_.gatewayId, &messageBuffer_, response);
                if(success)
                {
                    // Parse message

                    currentState->step++;   // Next step
                }
            }
        }
        else
        {
            // Timeout
            currentState->step++;   // Next step
        }
    }
    else if(currentState->step == 3)
    {
        stateLed_.cancel();
        success ? stateLed_.blinkInPeriod(1000, 0, 3000) : stateLed_.blinkNumberOfTimes(500, 500, 5);
        currentState->step++;
    }
    else if(currentState->step == 4)
    {
        // Led finished work
        if(stateLed_.getState() == LED_IDLE)
        {
            Log.verboseln(F("Led finished work"));
            currentState->result = success ? FSM_State::Result::Success : FSM_State::Result::Failed;
        }
    }
}

void Node::onLeaveRadioPairing()
{
    onLeaveActiveStateName();

    // Sommit EEPROM if success
    if(stateMachine_.CurrentState()->result == FSM_State::Result::Success)
    {
        Log.verboseln(F("pairing success!"));
    }
    else if(stateMachine_.CurrentState()->result == FSM_State::Result::Failed)
    {
        Log.warningln(F("pairing failed!"));
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

// Helpers
uint8_t Node::getRandomNodeIdForRegister()
{
    return TrueRandom.random(15, 3);
}

#endif