#include "unity.h"

#include "test_RadioConfig.hpp"
#include "test_UUIDConfig.hpp"
#include "test_MessageBuffer.hpp"

// Messages
#include "CommonBuffer.hpp"
#include "messages/radio/test_RadioMessage.hpp"
#include "messages/radio/test_RadioRequest.hpp"
#include "messages/radio/test_RadioResponse.hpp"
#include "messages/radio/test_RadioModelRequest.hpp"
#include "messages/radio/test_RadioModelResponse.hpp"
#include "messages/radio/test_RadioPairRequest.hpp"
#include "messages/radio/test_RadioPairResponse.hpp"
#include "messages/radio/test_RadioRegisterRequest.hpp"
#include "messages/radio/test_RadioRegisterResponse.hpp"
#include "messages/radio/test_RadioBatteryStateRequest.hpp"
#include "messages/radio/test_RadioBatteryStateResponse.hpp"

void setUp(void)
{
    buffer.clear();
#ifndef ARDUINO
    

    // Set EEPROM Mock allocation
    EEPROM.begin(512);

    // Set all memory cells to 0xff
    for(int i = 0; i < 512; ++i)
    {
        EEPROM.write(i, 0xff);
    }

#else
    //ArduinoFakeReset();
#endif

#ifdef ESP32
    EEPROM.begin(512);
#endif
}

void tearDown(void) {
#ifndef ARDUINO
    // Clear EEPROM Mock allocation
    EEPROM.end();
#endif

#ifdef ESP32
    EEPROM.end();
#endif
}

void process() {
    UNITY_BEGIN();

    // run_tests_RadioConfig();
    // run_tests_UUIDConfig();
    // run_tests_MessageBuffer();

    // Messages
    //run_tests_RadioMessage();
    //run_tests_RadioRequest();
    //run_tests_RadioResponse();
    run_tests_RadioModelRequest();
    run_tests_RadioModelResponse();
    run_tests_RadioPairRequest();
    run_tests_RadioPairResponse();
    run_tests_RadioRegisterRequest();
    run_tests_RadioRegisterResponse();
    run_tests_RadioBatteryStateRequest();
    run_tests_RadioBatteryStateResponse();

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