#if defined(__AVR__)

#include <Arduino.h>
#include <ArduinoLog.h>
#include "Node.hpp"
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
        if(isRadioPairBtnTriggered())
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
    if(isRadioPairBtnTriggered())
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

#endif