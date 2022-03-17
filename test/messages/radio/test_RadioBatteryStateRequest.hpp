#pragma once
#include "unity.h"
#include "messages/radio/concrete/RadioBatteryStateRequest.hpp"
#include "../../CommonBuffer.hpp"

namespace Statics_RadioBatteryStateRequest
{
    static const auto eType = RadioMessageType::BatteryState;
    static const auto eDirection = MessageDirection::Request;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);
};

void test_RadioBatteryStateRequest_build()
{
    //MessageBuffer buffer;

    RadioBatteryStateRequest msg(buffer);
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d", Statics_RadioBatteryStateRequest::iType, Statics_RadioBatteryStateRequest::iDirection);
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioBatteryStateRequest_parse_ok()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioBatteryStateRequest::iType);
    buffer.appendInt(Statics_RadioBatteryStateRequest::iDirection, true);

    RadioBatteryStateRequest msg(buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioBatteryStateRequest::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioBatteryStateRequest::eDirection, msg.direction());
}

void test_RadioBatteryStateRequest_parse_error()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioBatteryStateRequest::iDirection, true);

    RadioBatteryStateRequest msg(buffer);
    TEST_ASSERT_FALSE(msg.parse());
}

void run_tests_RadioBatteryStateRequest() {
    RUN_TEST(test_RadioBatteryStateRequest_build);
    RUN_TEST(test_RadioBatteryStateRequest_parse_ok);
    RUN_TEST(test_RadioBatteryStateRequest_parse_error);
}