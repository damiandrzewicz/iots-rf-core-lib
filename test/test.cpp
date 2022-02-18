#include <unity.h>
#include "test_RadioConfig.hpp"
#include "test_UUIDConfig.hpp"
//#include "test_NodeCore.hpp"

void setUp(void)
{
#ifndef ARDUINO
    ArduinoFakeReset();

    // Set EEPROM Mock allocation
    EEPROM.begin(512);

    // Set all memory cells to 0xff
    for(int i = 0; i < 512; ++i)
    {
        EEPROM.write(i, 0xff);
    }
#endif
}

void tearDown(void) {
#ifndef ARDUINO
    // Clear EEPROM Mock allocation
    EEPROM.end();
#endif
}

void process() {
    UNITY_BEGIN();

    run_tests_RadioConfig();
    run_tests_UUIDConfig();
    //run_tests_NodeCore();

    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    process();
}

void loop() {
    digitalWrite(9, HIGH);
    delay(100);
    digitalWrite(9, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    process();
    return 0;
}

#endif