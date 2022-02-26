#if defined(__AVR__)

#include <Arduino.h>
#include <ArduinoLog.h>
#include "Node.hpp"

const char* const stateName[] PROGMEM = { 
    "VerifyConfig", "Pairing", "RadioListen", "SendState", "Sleep"
};

void Node::setupStateMachine()
{
    Log.verboseln(F("Node::setupStateMachine"));

    /** States */

    stateMachine_.AddState(stateName[static_cast<int>(State::VerifyConfig)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){  }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::Pairing)], 5000L,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); },
        [](void *ctx){ static_cast<Node*>(ctx)->onPairing(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::RadioListen)], 2000,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onRadioListen(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::SendState)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onSendState(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    stateMachine_.AddState(stateName[static_cast<int>(State::Sleep)], 0,
        [](void *ctx){ static_cast<Node*>(ctx)->onEnterActiveStateName(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onSleep(); }, 
        [](void *ctx){ static_cast<Node*>(ctx)->onLeaveActiveStateName(); }, 
        this
    );

    /** Transitions */

    stateMachine_.AddTransition(static_cast<int>(State::VerifyConfig), static_cast<int>(State::Pairing), 
        [](void *ctx){
            return  
                static_cast<Node*>(ctx)->checkButton() && 
                static_cast<Node*>(ctx)->radioConfig_.isEmpty() &&
                static_cast<Node*>(ctx)->uuidConfig_.isEmpty();
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::VerifyConfig), static_cast<int>(State::SendState), 
        [](void *ctx){
            return 
                !static_cast<Node*>(ctx)->radioConfig_.isEmpty() &&
                !static_cast<Node*>(ctx)->uuidConfig_.isEmpty();
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::Pairing), static_cast<int>(State::VerifyConfig), 
        [](void *ctx){
            return static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Failed;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::Pairing), static_cast<int>(State::SendState), 
        [](void *ctx){
            return static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::SendState), static_cast<int>(State::RadioListen), 
        [](void *ctx){
            return static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Success;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::SendState), static_cast<int>(State::Sleep), 
        [](void *ctx){
            return static_cast<Node*>(ctx)->stateMachine_.CurrentState()->Result == FSM_State::Result::Failed;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioListen), static_cast<int>(State::Sleep), 
        [](void *ctx){
            return static_cast<Node*>(ctx)->stateMachine_.CurrentState()->timeout;
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::Sleep), static_cast<int>(State::Pairing), 
        [](void *ctx){
            return  
                static_cast<Node*>(ctx)->checkButton();
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::SendState), static_cast<int>(State::Pairing), 
        [](void *ctx){
            return  
                static_cast<Node*>(ctx)->checkButton();
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::RadioListen), static_cast<int>(State::Pairing), 
        [](void *ctx){
            return  
                static_cast<Node*>(ctx)->checkButton();
        },
        this    
    );

    stateMachine_.AddTransition(static_cast<int>(State::Sleep), static_cast<int>(State::SendState), 
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
    pinMode(statusLed_, OUTPUT);
    pinMode(radioPairBtn_, INPUT_PULLUP);

    radioConfig_.clear();
    radioConfig_.save();

    uuidConfig_.clear();
    uuidConfig_.save();

    radioConfig_.read();
    uuidConfig_.read();
}

void Node::onPairing()
{
    auto currState = stateMachine_.CurrentState();
    auto success = true;

    if(!currState->timeout)
    {
        statusLedBlink(120L);

        bool success = true;
        static uint32_t execTime = 0;
        if(millis() - execTime > 1000L){
            execTime = millis();
            Log.verboseln(F("do some stuff..."));
        }
    }
    else if( success ? !statusLedBlink(0, 3000L) : !statusLedBlink(500L, 3000L) )
    {
        currState->Result = success ? FSM_State::Result::Success : FSM_State::Result::Failed;
    }
}

void Node::onRadioListen()
{
    //Log.verboseln(F("Node::onRadioListen"));
    
}

void Node::onSendState()
{
    Log.verboseln(F("Node::onSendState"));
    stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
}

void Node::onSleep()
{
    Log.verboseln(F("Node::onSleep"));

    deepSleepFor(3000);
    stateMachine_.CurrentState()->Result = FSM_State::Result::Success;
}

bool Node::checkButton()
{
    static int lastFlickerableState = HIGH;  // the previous flickerable state from the input pin
    static unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
    static const int DEBOUNCE_DELAY = 50;
    int currentState = digitalRead(radioPairBtn_);               // the current reading from the input pin

    // If the switch/button changed, due to noise or pressing:
    if (currentState != lastFlickerableState) {
        // reset the debouncing timer
        lastDebounceTime = millis();
        // save the the last flickerable state
        lastFlickerableState = currentState;
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (currentState == LOW)
        {
            lastFlickerableState = HIGH;
            return true;
        }
    }
    return false;
}

bool Node::statusLedBlink(int delay, int execFor)
{
    static uint32_t ledTime = 0;
	if(delay && (millis() - ledTime > delay)){
		ledTime = millis();
		digitalWrite(statusLed_, ! digitalRead(statusLed_));
	}
    else if(!delay)
    {
        digitalWrite(statusLed_, HIGH);
    }

    if(execFor)
    {
        static uint32_t startTime = 0;
        if(!startTime)
        {
            startTime = millis();
        }

        if(millis() - startTime > execFor)
        {
            startTime = 0;
            digitalWrite(statusLed_, LOW);
            return false;
        }
    }
    return true;
}

#endif