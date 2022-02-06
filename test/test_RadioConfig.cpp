#include <unity.h>

#include "RadioConfig.hpp"

void setUp(void)
{
    ArduinoFakeReset();

    // Set EEPROM Mock allocation
    EEPROM.begin(512);

    // Set all memory cells to 0xff
    for(int i = 0; i < 512; ++i)
    {
        EEPROM.write(i, 0xff);
    }
}

void tearDown(void) {
    // Clear EEPROM Mock allocation
    EEPROM.end();
}

void test_RadioConfig_readUndefined()
{
    RadioConfig rc(0);
    rc.read();
    TEST_ASSERT_TRUE(rc.isEmpty());
}

void test_RadioConfig_setDefaults()
{
    RadioConfig rc(0);
    rc.setDefaults();
    auto &data = rc.data();

    TEST_ASSERT_EQUAL(100, data.networkId);
    TEST_ASSERT_EQUAL(1, data.gatewayId);
    TEST_ASSERT_EQUAL(0, data.customFrequency);
    TEST_ASSERT_TRUE(!strcmp("sampleEncryptKey", data.encryptKey));
    TEST_ASSERT_EQUAL(0, data.powerLevel);
}

void test_RadioConfig_saveDefaults()
{
    RadioConfig rc(0);
    rc.setDefaults();
    rc.save();
    rc.read();

    TEST_ASSERT_FALSE(rc.isEmpty());

    auto &data = rc.data();

    TEST_ASSERT_EQUAL(100, data.networkId);
    TEST_ASSERT_EQUAL(1, data.gatewayId);
    TEST_ASSERT_EQUAL(0, data.customFrequency);
    TEST_ASSERT_TRUE(!strcmp("sampleEncryptKey", data.encryptKey));
    TEST_ASSERT_EQUAL(0, data.powerLevel);
}

void test_RadioConfig_saveCustom()
{
    RadioConfig rc(0);
    auto &data = rc.data();

    data.networkId = 123;
    data.gatewayId = 77;
    data.customFrequency = 869000000;
    strcpy(data.encryptKey, "test123");
    data.powerLevel = 20;

    rc.save();
    rc.read();

    TEST_ASSERT_FALSE(rc.isEmpty());

    auto &saved = rc.data();

    TEST_ASSERT_EQUAL(123, saved.networkId);
    TEST_ASSERT_EQUAL(77, saved.gatewayId);
    TEST_ASSERT_EQUAL(869000000, saved.customFrequency);
    TEST_ASSERT_TRUE(!strcmp("test123", saved.encryptKey));
    TEST_ASSERT_EQUAL(20, saved.powerLevel);
}

void test_RadioConfig_clear()
{
    RadioConfig rc(0);
    rc.setDefaults();
    rc.save();
    rc.read();

    TEST_ASSERT_FALSE(rc.isEmpty());

    rc.clear();
    rc.save();

    TEST_ASSERT_TRUE(rc.isEmpty());
}

void test_request(void)
{
    // When(Method(ArduinoFake(Client), stop)).AlwaysReturn();
    // When(Method(ArduinoFake(Client), available)).Return(1, 1, 1, 0);
    // When(OverloadedMethod(ArduinoFake(Client), read, int())).Return(2, 0, 0);
    // When(OverloadedMethod(ArduinoFake(Client), connect, int(const char*, uint16_t))).Return(1);

    // Client* clientMock = ArduinoFakeMock(Client);

    // MyService service(clientMock);

    // String response = service.request("myserver.com");

    // TEST_ASSERT_EQUAL(3, response.length());
    // TEST_ASSERT_TRUE(response.equals("200"));

    // Verify(Method(ArduinoFake(Client), stop)).Once();
    // Verify(Method(ArduinoFake(Client), available)).Exactly(4_Times);
    // Verify(OverloadedMethod(ArduinoFake(Client), read, int())).Exactly(3_Times);
    // Verify(OverloadedMethod(ArduinoFake(Client), connect, int(const char*, uint16_t)).Using("myserver.com", 80)).Once();
}

void process() {
    UNITY_BEGIN();

    RUN_TEST(test_RadioConfig_readUndefined);
    RUN_TEST(test_RadioConfig_setDefaults);
    RUN_TEST(test_RadioConfig_saveDefaults);
    RUN_TEST(test_RadioConfig_saveCustom);
    RUN_TEST(test_RadioConfig_clear);

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
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    process();
    return 0;
}

#endif