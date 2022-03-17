#pragma once
#include "unity.h"
#include "messages/radio/concrete/RadioBatteryStateResponse.hpp"
#include "../../CommonBuffer.hpp"

namespace Statics_RadioBatteryStateResponse
{
    static const auto eType = RadioMessageType::BatteryState;
    static const auto eDirection = MessageDirection::Response;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);
};

void test_RadioBatteryStateResponse_build_resultOk()
{
    // MessageBuffer buffer;
    RadioBatteryStateResponse msg(buffer);

    auto &model = msg.model();
    model.batteryPercentValue = -55;

    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d|%d",
        Statics_RadioBatteryStateResponse::iType,
        Statics_RadioBatteryStateResponse::iDirection,
        static_cast<uint8_t>(RadioBatteryStateResponse::Result::Success),
        model.batteryPercentValue
    );

    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}


void test_RadioBatteryStateResponse_build_resultError()
{
    // MessageBuffer buffer;
    RadioBatteryStateResponse msg(buffer);
    msg.setError();
    
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d",
        Statics_RadioBatteryStateResponse::iType,
        Statics_RadioBatteryStateResponse::iDirection,
        static_cast<uint8_t>(RadioBatteryStateResponse::Result::Error)
    );

    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioBatteryStateResponse_parse_ok()
{
    uint8_t value = 45;
    
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioBatteryStateResponse::iType);
    buffer.appendInt(Statics_RadioBatteryStateResponse::iDirection);
    buffer.appendInt(static_cast<uint8_t>(RadioBatteryStateResponse::Result::Success));
    buffer.appendInt(value, true);

    RadioBatteryStateResponse msg(buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioBatteryStateResponse::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioBatteryStateResponse::eDirection, msg.direction());
    TEST_ASSERT_EQUAL(msg.getResult(), static_cast<uint8_t>(RadioBatteryStateResponse::Result::Success));
    TEST_ASSERT_EQUAL(value, msg.model().batteryPercentValue);
}

void test_RadioBatteryStateResponse_parse_error()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioBatteryStateResponse::iType);
    buffer.appendInt(Statics_RadioBatteryStateResponse::iDirection);
    buffer.appendInt(static_cast<uint8_t>(RadioBatteryStateResponse::Result::Success));

    RadioBatteryStateResponse msg(buffer);
    TEST_ASSERT_FALSE(msg.parse());
}

void run_tests_RadioBatteryStateResponse() {
    RUN_TEST(test_RadioBatteryStateResponse_build_resultOk);
    RUN_TEST(test_RadioBatteryStateResponse_build_resultError);
    RUN_TEST(test_RadioBatteryStateResponse_parse_ok);
    RUN_TEST(test_RadioBatteryStateResponse_parse_error);
}