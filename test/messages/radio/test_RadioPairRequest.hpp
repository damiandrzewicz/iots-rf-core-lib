#pragma once
#include "unity.h"
#include "messages/radio/concrete/RadioPairRequest.hpp"

namespace Statics_RadioPairRequest
{
    static const uint8_t iType = static_cast<uint8_t>(RadioMessageType::Pair);
    static const uint8_t iDirection = static_cast<uint8_t>(MessageDirection::Request);
    static const auto eType = RadioMessageType::Pair;
    static const auto eDirection = MessageDirection::Request;
};

void test_RadioPairRequest_build()
{
    MessageBuffer buffer;

    RadioPairRequest msg(buffer);
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d", Statics_RadioPairRequest::iType, Statics_RadioPairRequest::iDirection);
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioPairRequest_parse_ok()
{
    MessageBuffer buffer;
    buffer.appendInt(Statics_RadioPairRequest::iType);
    buffer.appendInt(Statics_RadioPairRequest::iDirection, true);

    RadioPairRequest msg(buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioPairRequest::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioPairRequest::eDirection, msg.direction());
}

void test_RadioPairRequest_parse_error()
{
    MessageBuffer buffer;
    buffer.appendInt(Statics_RadioPairRequest::iDirection, true);

    RadioPairRequest msg(buffer);
    TEST_ASSERT_FALSE(msg.parse());
}

void run_tests_RadioPairRequest() {
    RUN_TEST(test_RadioPairRequest_build);
    RUN_TEST(test_RadioPairRequest_parse_ok);
    RUN_TEST(test_RadioPairRequest_parse_error);
}