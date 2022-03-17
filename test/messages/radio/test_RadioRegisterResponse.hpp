#pragma once
#include "unity.h"
#include "messages/radio/concrete/RadioRegisterResponse.hpp"
#include "../../CommonBuffer.hpp"

namespace Statics_RadioRegisterResponse
{
    static const auto eType = RadioMessageType::Register;
    static const auto eDirection = MessageDirection::Response;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);
};

void test_RadioRegisterResponse_build_resultOk()
{
    // MessageBuffer buffer;
    RadioRegisterResponse msg(buffer);

    auto &model = msg.model();
    model.nodeId = -55;

    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d|%d",
        Statics_RadioRegisterResponse::iType,
        Statics_RadioRegisterResponse::iDirection,
        static_cast<uint8_t>(RadioRegisterResponse::Result::Success),
        model.nodeId
    );

    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}


void test_RadioRegisterResponse_build_resultError()
{
    // MessageBuffer buffer;
    RadioRegisterResponse msg(buffer);
    msg.setError();
    
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d",
        Statics_RadioRegisterResponse::iType,
        Statics_RadioRegisterResponse::iDirection,
        static_cast<uint8_t>(RadioRegisterResponse::Result::Error)
    );

    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioRegisterResponse_parse_ok()
{
    int8_t nodeId = 133;
    
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioRegisterResponse::iType);
    buffer.appendInt(Statics_RadioRegisterResponse::iDirection);
    buffer.appendInt(static_cast<uint8_t>(RadioRegisterResponse::Result::Success));
    buffer.appendInt(nodeId, true);

    RadioRegisterResponse msg(buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioRegisterResponse::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioRegisterResponse::eDirection, msg.direction());
    TEST_ASSERT_EQUAL(msg.getResult(), static_cast<uint8_t>(RadioResponse::Result::Success));
    TEST_ASSERT_EQUAL(nodeId, msg.model().nodeId);

}

void test_RadioRegisterResponse_parse_error()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioRegisterResponse::iType);
    buffer.appendInt(Statics_RadioRegisterResponse::iDirection);
    buffer.appendInt(static_cast<uint8_t>(RadioResponse::Result::Success));

    RadioRegisterResponse msg(buffer);
    TEST_ASSERT_FALSE(msg.parse());
}

void run_tests_RadioRegisterResponse() {
    RUN_TEST(test_RadioRegisterResponse_build_resultOk);
    RUN_TEST(test_RadioRegisterResponse_build_resultError);
    RUN_TEST(test_RadioRegisterResponse_parse_ok);
    RUN_TEST(test_RadioRegisterResponse_parse_error);
}