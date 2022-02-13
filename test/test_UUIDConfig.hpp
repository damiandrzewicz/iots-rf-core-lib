#pragma once
#include <unity.h>
#include "UUIDConfig.hpp"

void test_UUIDConfig_readUndefined()
{
    UUIDConfig rc(0);
    rc.read();
    TEST_ASSERT_TRUE(rc.isEmpty());
}

void test_UUIDConfig_setDefaults()
{
    UUIDConfig rc(0);
    rc.setDefaults();
    auto &data = rc.data();

    for(const auto &item : data.uuid)
    {
        TEST_ASSERT_EQUAL(0, item);
    }
}

void test_UUIDConfig_saveDefaults()
{
    UUIDConfig rc(0);
    rc.setDefaults();
    rc.save();
    rc.read();

    TEST_ASSERT_FALSE(rc.isEmpty());

    auto &data = rc.data();

    for(const auto &item : data.uuid)
    {
        TEST_ASSERT_EQUAL(0, item);
    }
}

void test_UUIDConfig_saveCustom()
{
    UUIDConfig rc(0);
    auto &data = rc.data();

    const char *uuid = "A1B2C3D4E5F6AABB";

    strcpy(reinterpret_cast<char*>(data.uuid), uuid);

    rc.save();
    rc.read();

    TEST_ASSERT_FALSE(rc.isEmpty());

    auto &saved = rc.data();

    TEST_ASSERT_TRUE(!strcmp(uuid, reinterpret_cast<const char*>(saved.uuid)));
}

void test_UUIDConfig_clear()
{
    UUIDConfig rc(0);
    rc.setDefaults();
    rc.save();
    rc.read();

    TEST_ASSERT_FALSE(rc.isEmpty());

    rc.clear();
    rc.save();

    TEST_ASSERT_TRUE(rc.isEmpty());
}

void run_tests_UUIDConfig() {
    RUN_TEST(test_UUIDConfig_readUndefined);
    RUN_TEST(test_UUIDConfig_setDefaults);
    RUN_TEST(test_UUIDConfig_saveDefaults);
    RUN_TEST(test_UUIDConfig_saveCustom);
    RUN_TEST(test_UUIDConfig_clear);
}