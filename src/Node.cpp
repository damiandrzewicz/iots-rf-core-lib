#include <Arduino.h>
#include <ArduinoLog.h>
#include "Node.hpp"
#include "utils/LowPowerWrapper.h"
#include <TimerOne.h>
#include "StatusLed.hpp"

StatusLed Node::statusLed = {9};

void timer1Handler()
{
    Node::statusLed.execISR();
}

void Node::setup()
{
    Log.noticeln(F("Node::setup IN"));

    // Setup pins
     pinMode(s_radioPairPin_, INPUT_PULLUP);

     Timer1.initialize(10000);
     Timer1.attachInterrupt(timer1Handler);
     statusLedBlink(-1);
    
    readConfiguration();

    radioConfig_.clear();
    radioConfig_.save();

    Log.noticeln(F("check radio config..."));

    // Wait until node configured
    while(radioConfig_.isEmpty())
    {
        if(isRadioPairTriggered())
        {
            radioPairRoutine();
            radioConfig_.setDefaults();
        }
    }

    Log.noticeln(F("radio configured!"));

    // If configuration available then configure radio
    radioSetup(radioConfig_.data());  

    Log.noticeln(F("Node::setup OUT"));
}

void Node::loop()
{
    // Pairing mode - button pressed
    if(isRadioPairTriggered())
    {
        radioPairRoutine();
        radioSetup(radioConfig_.data());  
    }

    // Work mode
}

void Node::readConfiguration()
{
    Log.verboseln(F("Node::readConfiguration"));

    Appliance::readConfiguration();

    // Read UUID EEPROM Config
    uuidConfig_.read();
}

bool Node::isRadioPairTriggered()
{
    static int lastFlickerableState = HIGH;  // the previous flickerable state from the input pin
    static unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
    static const int DEBOUNCE_DELAY = 3000;
    int currentState = digitalRead(s_radioPairPin_);               // the current reading from the input pin

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

// void Node::readConfiguration()
// {
//     Log.verboseln(F("Node::readConfiguration"));

//     // Read UUID EEPROM Config
//     uuidConfig_.read();
// }

// // ------------
// bool Node::isRadioConfigured()
// {
//     return !radioConfig_.isEmpty();
// }

// bool Node::isRadioPairing()
// {

// }

// bool Node::radioPairWorker()
// {
//     Log.verboseln(F("Node::radioPairWorker IN"));

//     // Setup radio
//     // ...


//     radioSetup(rcf);
//     statusLedBlink(10);

//     for(uint8_t i = 0; i < 8; ++i)
//     {
//         Log.noticeln(F("Pair attempt: %d"), i);

//         // Build radio pair message

//         // Send message and get response

//         // Parse response and decide about result

//         delay(1000);
//     }




//     Log.noticeln(F("Node::radioPairWorker OUT"));

//     return false;
// }



// Status Led
void Node::statusLedBlink(int16_t speed)
{
    if(speed)
    {
        statusLed.startBlink(speed);
        Timer1.start();
    }
    else if(!speed)
    {
        statusLed.startBlink(0);
    }
    else if(speed == -1)
    {
        statusLed.stopBlink();
        Timer1.stop();
    }
}

