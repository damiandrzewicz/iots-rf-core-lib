#pragma once
#include "unity.h"
#include "UUIDConfig.hpp"
#include <EEPROM.h>

void test_UUIDConfig_readUndefined()
{
    UUIDConfig rc;
    EEPROM.get(100, rc);
    TEST_ASSERT_TRUE(rc.isEmpty());
}

void test_UUIDConfig_setDefaults()
{
    UUIDConfig rc;
    rc.setDefaults();
}

void test_UUIDConfig_saveDefaults()
{
    UUIDConfig rc;
    EEPROM.get(100, rc);
    rc.setDefaults();

    EEPROM.put(100, rc);

    UUIDConfig data;
    EEPROM.get(100, data);

    TEST_ASSERT_FALSE(rc.isEmpty());
    TEST_ASSERT_EQUAL_INT8_ARRAY(rc.uuidNumber, data.uuidNumber, 16 );
}

void test_UUIDConfig_saveCustom()
{
    UUIDConfig rc;

    for(uint8_t i = 0; i < sizeof rc.uuidNumber; ++i)
    {
        rc.uuidNumber[i] = i*2;
    }

    EEPROM.put(100, rc);
    
    UUIDConfig saved;
    EEPROM.get(100, saved);

    TEST_ASSERT_FALSE(rc.isEmpty());
    TEST_ASSERT_EQUAL_INT8_ARRAY(rc.uuidNumber, saved.uuidNumber, 16 );
}

void test_UUIDConfig_clear()
{
    UUIDConfig rc;
    EEPROM.get(100, rc);
    rc.setDefaults();
    EEPROM.put(100, rc);

    EEPROM.get(100, rc);

    TEST_ASSERT_FALSE(rc.isEmpty());

    rc.setEmpty();
    EEPROM.put(100, rc);

    TEST_ASSERT_TRUE(rc.isEmpty());
}

void test_UUIDConfig_dataSize()
{
    UUIDConfig rc;
    TEST_ASSERT_EQUAL(sizeof(UUIDConfig), rc.size());
}

void run_tests_UUIDConfig() {
    //RUN_TEST(test_UUIDConfig_readUndefined);
    RUN_TEST(test_UUIDConfig_setDefaults);
    RUN_TEST(test_UUIDConfig_saveDefaults);
    RUN_TEST(test_UUIDConfig_saveCustom);
    RUN_TEST(test_UUIDConfig_clear);
    RUN_TEST(test_UUIDConfig_dataSize);
}