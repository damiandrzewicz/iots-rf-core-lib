#include <Arduino.h>
#include <ArduinoLog.h>
#include "NodeCore.hpp"
#include "utils/LowPowerWrapper.h"

uint8_t NodeCore::s_ledBlinkSpeed = 0;

void NodeCore::setup()
{
    // Initialize UART interface
    Serial.begin(115200);

    // Initialize logger
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    Log.noticeln(F("NodeCore::setup"));

    // Setup pins
    pinMode(s_radioPairPin_, INPUT_PULLUP);
    pinMode(s_ledPin, OUTPUT);

    setupLedTimer();
    readConfiguration();

    // Wait until node configured

    // while(!isRadioConfigured())
    // {
    //     if(isRadioPairing())
    //     {
    //         radioPairWorker();
    //     }
    // }

    // If configuration available then configure radio
    // setupRadio(radioConfig_.data());
    // while(!registerNode())
    // {
    //     // Try to register
    // }    
}

void NodeCore::loop()
{
    // Pairing mode - button pressed
    if(isRadioPairing())
    {
        radioPairWorker();
        setupRadio(radioConfig_.data());
    }

    // Work mode
}

void NodeCore::readConfiguration()
{
    Log.noticeln(F("NodeCore::readConfiguration"));

    // Read Radio EEPROM config
    radioConfig_.read();

    // Read UUID EEPROM Config
    uuidConfig_.read();
}

// ------------
bool NodeCore::isRadioConfigured()
{
    return !radioConfig_.isEmpty();
}

bool NodeCore::isRadioPairing()
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

bool NodeCore::radioPairWorker()
{
    Log.noticeln(F("NodeCore::radioPairWorker IN"));

    // Setup radio
    // ...
    RadioConfigData rcf;
    rcf.gatewayId = 1;
    rcf.networkId = 111;
    rcf.customFrequency = 869000000;
    strcpy_P(rcf.encryptKey, PSTR("sampleEncryptKey"));
    rcf.powerLevel = 0;

    setupRadio(rcf);

    setLedMode(LedMode::Pair);

    for(uint8_t i = 0; i < 8; ++i)
    {
        Log.noticeln(F("Pair attempt: %d"), i);

        delay(1000);
    }

    setLedMode(LedMode::Error);

    Log.noticeln(F("NodeCore::radioPairWorker OUT"));

    return false;
}

void NodeCore::setupRadio(const RadioConfigData &data)
{
    Log.noticeln(F("NodeCore::setupRadio"));
}

bool NodeCore::registerNode()
{
    Log.noticeln(F("NodeCore::registerNode"));
    return false;
}

void NodeCore::deepSleepDelay(unsigned int delay_ms)
{
    Log.noticeln(F("NodeCore::deepSleepDelay for %d ms"), delay_ms);
    Serial.flush();
    //radio_.sleep();
    LowPowerWrp.DeepSleep(delay_ms);
}

void wakeUp()
{
    //DeviceCore::IsExtInterrupt = true;
}

void NodeCore::deepSleepForewerAndWakeInt(uint8_t pin, uint8_t mode)
{
    Log.noticeln(F("NodeCore::deepSleepForewerAndWakeInt, pin: %d, mode: %d"), pin, mode);
    Serial.flush();
    radio_.sleep();
    attachInterrupt(digitalPinToInterrupt(pin), wakeUp, mode);
    //IsExtInterrupt = false;    // Clear interrupt flag
    LowPowerWrp.DeepSleepForever();
    detachInterrupt(0); 
}

// Led timer
void NodeCore::setLedMode(NodeCore::LedMode mode)
{
    enableLedTimer();

    if(mode == LedMode::Pair)
    {
        s_ledBlinkSpeed = 12;
    }
    else
    {
        if(mode == LedMode::Success)
        {
            s_ledBlinkSpeed = 0;
            delay(3000);
            disableLedTimer();
        }
        else if(mode == LedMode::Error)
        {
            s_ledBlinkSpeed = 50;
            delay(3000);
            disableLedTimer();
        }

        // Make sure LED is off
        digitalWrite(s_ledPin, LOW);
    }
}

void NodeCore::setupLedTimer()
{
    cli();//stop interrupts
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    OCR1A = 70;   // 100Hz (7380000 / (1024*100) - 1)
    TCCR1B |= (1 << WGM12);   // turn on CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10);    // Set CS01 and CS00 bits for 1024 prescaler
    sei();//allow interrupts
}

void NodeCore::enableLedTimer()
{
    TIMSK1 |= (1 << OCIE1A);
}

void NodeCore::disableLedTimer()
{
    TIMSK1 &= ~(1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
    static uint8_t tick = 0;
    static boolean toggle = true;

    if(NodeCore::s_ledBlinkSpeed && ++tick < NodeCore::s_ledBlinkSpeed)
    {
        return;
    }

    if (toggle){
        digitalWrite(NodeCore::s_ledPin, HIGH);
        toggle = 0;
    }
    else{
        digitalWrite(NodeCore::s_ledPin, LOW);
        toggle = 1;
    }

    tick = 0;
}