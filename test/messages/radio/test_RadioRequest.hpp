#pragma once
#include "unity.h"
#include "messages/radio/RadioRequest.hpp"
#include "../../CommonBuffer.hpp"

namespace Statics_RadioRequest
{
    static const auto eType = RadioMessageType::Alive;
    static const auto eDirection = MessageDirection::Request;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);
};

void test_RadioRequest_build()
{
    // MessageBuffer buffer;

    RadioRequest msg(Statics_RadioRequest::eType, buffer);
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d", Statics_RadioRequest::iType, Statics_RadioRequest::iDirection);
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioRequest_parse_ok()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioRequest::iType);
    buffer.appendInt(Statics_RadioRequest::iDirection, true);

    RadioRequest msg(Statics_RadioRequest::eType, buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioRequest::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioRequest::eDirection, msg.direction());
}

void test_RadioRequest_parse_error()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioRequest::iDirection, true);

    RadioRequest msg(Statics_RadioRequest::eType, buffer);
    TEST_ASSERT_FALSE(msg.parse());
}

void run_tests_RadioRequest() {
    RUN_TEST(test_RadioRequest_build);
    RUN_TEST(test_RadioRequest_parse_ok);
    RUN_TEST(test_RadioRequest_parse_error);
}