#pragma once
#include "unity.h"
#include "messages/radio/RadioMessage.hpp"
#include "../../CommonBuffer.hpp"

namespace Statics_RadioMessage
{
    static const auto eType = RadioMessageType::Alive;
    static const auto eDirection = MessageDirection::Response;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);
};

void test_RadioMessage_build()
{
    //MessageBuffer buffer;

    RadioMessage msg({ Statics_RadioMessage::eType, Statics_RadioMessage::eDirection }, buffer);
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d", Statics_RadioMessage::iType, Statics_RadioMessage::iDirection);
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioMessage_parse_ok()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioMessage::iType);
    buffer.appendInt(Statics_RadioMessage::iDirection, true);

    RadioMessage msg({ Statics_RadioMessage::eType, Statics_RadioMessage::eDirection }, buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioMessage::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioMessage::eDirection, msg.direction());
}

void test_RadioMessage_parse_error()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioMessage::iDirection, true);

    RadioMessage msg({ Statics_RadioMessage::eType, Statics_RadioMessage::eDirection }, buffer);
    TEST_ASSERT_FALSE(msg.parse());
}

void run_tests_RadioMessage() {
    RUN_TEST(test_RadioMessage_build);
    RUN_TEST(test_RadioMessage_parse_ok);
    RUN_TEST(test_RadioMessage_parse_error);
}