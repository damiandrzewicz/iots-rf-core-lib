#pragma once
#include "unity.h"
#include "RadioConfig.hpp"
#include <EEPROM.h>

void test_RadioConfig_readUndefined()
{
    RadioConfig rc;
    EEPROM.get(0, rc);
    TEST_ASSERT_TRUE(rc.isEmpty());
}

void test_RadioConfig_setDefaults()
{
    RadioConfig rc;
    rc.setDefaults();

    TEST_ASSERT_EQUAL(100, rc.networkId);
    TEST_ASSERT_EQUAL(1, rc.gatewayId);
    TEST_ASSERT_EQUAL(0, rc.customFrequency);
    TEST_ASSERT_TRUE(!strcmp("sampleEncryptKey", rc.encryptKey));
    TEST_ASSERT_EQUAL(-80, rc.rssi);
}

void test_RadioConfig_saveDefaults()
{
    RadioConfig rc;
    EEPROM.get(0, rc);
    rc.setDefaults();

    EEPROM.put(0, rc);

    RadioConfig data;
    EEPROM.get(0, data);

    TEST_ASSERT_FALSE(rc.isEmpty());

    TEST_ASSERT_EQUAL(100, data.networkId);
    TEST_ASSERT_EQUAL(1, data.gatewayId);
    TEST_ASSERT_EQUAL(0, data.customFrequency);
    TEST_ASSERT_TRUE(!strcmp("sampleEncryptKey", data.encryptKey));
    TEST_ASSERT_EQUAL(-80, rc.rssi);
}

void test_RadioConfig_saveCustom()
{
    RadioConfig rc;

    rc.networkId = 123;
    rc.gatewayId = 77;
    rc.customFrequency = 869000000;
    strcpy(rc.encryptKey, "test123");
    rc.rssi = -20;

    EEPROM.put(0, rc);
    
    RadioConfig saved;
    EEPROM.get(0, saved);

    TEST_ASSERT_FALSE(rc.isEmpty());

    TEST_ASSERT_EQUAL(123, saved.networkId);
    TEST_ASSERT_EQUAL(77, saved.gatewayId);
    TEST_ASSERT_EQUAL(869000000, saved.customFrequency);
    TEST_ASSERT_TRUE(!strcmp("test123", saved.encryptKey));
    TEST_ASSERT_EQUAL(-20, saved.rssi);
}

void test_RadioConfig_clear()
{
    RadioConfig rc;
    EEPROM.get(0, rc);
    rc.setDefaults();
    EEPROM.put(0, rc);

    RadioConfig data;
    EEPROM.get(0, rc);

    TEST_ASSERT_FALSE(rc.isEmpty());

    rc.setEmpty();
    EEPROM.put(0, rc);

    TEST_ASSERT_TRUE(rc.isEmpty());
}

void test_RadioConfig_dataSize()
{
    RadioConfig rc;
    TEST_ASSERT_EQUAL(sizeof(RadioConfig), rc.size());
}

void run_tests_RadioConfig() {
    //RUN_TEST(test_RadioConfig_readUndefined);
    RUN_TEST(test_RadioConfig_setDefaults);
    RUN_TEST(test_RadioConfig_saveDefaults);
    RUN_TEST(test_RadioConfig_saveCustom);
    RUN_TEST(test_RadioConfig_clear);
    RUN_TEST(test_RadioConfig_dataSize);
}