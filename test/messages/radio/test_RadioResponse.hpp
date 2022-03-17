#pragma once
#include "unity.h"
#include "messages/radio/RadioResponse.hpp"
#include "../../CommonBuffer.hpp"

namespace Statics_RadioResponse
{
    static const auto eType = RadioMessageType::Alive;
    static const auto eDirection = MessageDirection::Response;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);
};

void test_RadioResponse_build_ok()
{
    // MessageBuffer buffer;

    RadioResponse msg(Statics_RadioResponse::eType, buffer);
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d", Statics_RadioResponse::iType, Statics_RadioResponse::iDirection, (int)RadioResponse::Result::Success);
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioResponse_build_error()
{
    // MessageBuffer buffer;

    RadioResponse msg(Statics_RadioResponse::eType, buffer);
    msg.setError();
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d", Statics_RadioResponse::iType, Statics_RadioResponse::iDirection, (int)RadioResponse::Result::Error);
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioResponse_build_detailedError()
{
    // MessageBuffer buffer;

    RadioResponse msg(Statics_RadioResponse::eType, buffer);
    msg.setError(RadioResponse::Result::ParserError);
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d", Statics_RadioResponse::iType, Statics_RadioResponse::iDirection, (int)RadioResponse::Result::ParserError);
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioResponse_parse_ok()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioResponse::iType);
    buffer.appendInt(Statics_RadioResponse::iDirection);
    buffer.appendInt((int)RadioResponse::Result::Success);

    RadioResponse msg(Statics_RadioResponse::eType, buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioResponse::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioResponse::eDirection, msg.direction());
    TEST_ASSERT_FALSE(msg.isError());
}

void test_RadioResponse_parse_error()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioResponse::iType);
    buffer.appendInt(Statics_RadioResponse::iDirection);
    buffer.appendInt((int)RadioResponse::Result::Error);

    RadioResponse msg(Statics_RadioResponse::eType, buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioResponse::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioResponse::eDirection, msg.direction());
    TEST_ASSERT_TRUE(msg.isError());
}

void test_RadioResponse_parse_detailedError()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioResponse::iType);
    buffer.appendInt(Statics_RadioResponse::iDirection);
    buffer.appendInt((int)RadioResponse::Result::ParserError);

    RadioResponse msg(Statics_RadioResponse::eType, buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioResponse::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioResponse::eDirection, msg.direction());
    TEST_ASSERT_TRUE(msg.isError());
    TEST_ASSERT_EQUAL(RadioResponse::Result::ParserError, msg.getResult());
}

void run_tests_RadioResponse() {
    RUN_TEST(test_RadioResponse_build_ok);
    RUN_TEST(test_RadioResponse_build_error);
    RUN_TEST(test_RadioResponse_build_detailedError);
    RUN_TEST(test_RadioResponse_parse_ok);
    RUN_TEST(test_RadioResponse_parse_error);
    RUN_TEST(test_RadioResponse_parse_detailedError);
}