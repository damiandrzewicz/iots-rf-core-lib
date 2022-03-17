#pragma once
#include "unity.h"
#include "messages/radio/concrete/RadioRegisterRequest.hpp"
#include "../../CommonBuffer.hpp"

namespace Statics_RadioRegisterRequest
{
    static const auto eType = RadioMessageType::Register;
    static const auto eDirection = MessageDirection::Request;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);
};

void test_RadioRegisterRequest_build()
{
    // MessageBuffer buffer;

    const char *uuid = "testUuid";

    RadioRegisterRequest msg(buffer);
    msg.model().uuid = uuid;
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%s", 
        Statics_RadioRegisterRequest::iType, 
        Statics_RadioRegisterRequest::iDirection,
        uuid
    );
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioRegisterRequest_parse_ok()
{
    const char *uuid = "testUuid";

    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioRegisterRequest::iType);
    buffer.appendInt(Statics_RadioRegisterRequest::iDirection);
    buffer.appendText(uuid, true);

    RadioRegisterRequest msg(buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioRegisterRequest::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioRegisterRequest::eDirection, msg.direction());
    TEST_ASSERT_EQUAL_STRING(uuid, msg.model().uuid);
}

void test_RadioRegisterRequest_parse_error()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioRegisterRequest::iDirection, true);

    RadioRegisterRequest msg(buffer);
    TEST_ASSERT_FALSE(msg.parse());
}

void run_tests_RadioRegisterRequest() {
    RUN_TEST(test_RadioRegisterRequest_build);
    RUN_TEST(test_RadioRegisterRequest_parse_ok);
    RUN_TEST(test_RadioRegisterRequest_parse_error);
}