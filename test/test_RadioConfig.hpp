#pragma once
#include <unity.h>
#include "RadioConfig.hpp"

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

void test_RadioConfig_dataSize()
{
    RadioConfig rc(0);
    char buff[50];
    sprintf(buff, "RadioConfig size=[%zu]", rc.dataSize());
    TEST_MESSAGE(buff);
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

void run_tests_RadioConfig() {
    RUN_TEST(test_RadioConfig_readUndefined);
    RUN_TEST(test_RadioConfig_setDefaults);
    RUN_TEST(test_RadioConfig_saveDefaults);
    RUN_TEST(test_RadioConfig_saveCustom);
    RUN_TEST(test_RadioConfig_clear);
    RUN_TEST(test_RadioConfig_dataSize);
}