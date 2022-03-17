#pragma once
#include "unity.h"
#include "messages/radio/concrete/RadioPairResponse.hpp"
#include "../../CommonBuffer.hpp"

namespace Statics_RadioPairResponse
{
    static const auto eType = RadioMessageType::Pair;
    static const auto eDirection = MessageDirection::Response;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);
};

void test_RadioPairResponse_build_resultOk()
{
    // MessageBuffer buffer;
    RadioPairResponse msg(buffer);

    auto &model = msg.model();
    model.customFrequency = 123321123U;
    model.encryptKey = "test123456";
    model.gatewayId = 11;
    model.networkId = 99;
    model.rssi = -55;

    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d|%d|%d|%lu|%s|%d",
        Statics_RadioPairResponse::iType,
        Statics_RadioPairResponse::iDirection,
        static_cast<uint8_t>(RadioPairResponse::Result::Success),
        model.gatewayId,
        model.networkId,
        model.customFrequency,
        model.encryptKey,
        model.rssi
    );

    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}


void test_RadioPairResponse_build_resultError()
{
    // MessageBuffer buffer;
    RadioPairResponse msg(buffer);
    msg.setError();
    
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d",
        Statics_RadioPairResponse::iType,
        Statics_RadioPairResponse::iDirection,
        static_cast<uint8_t>(RadioPairResponse::Result::Error)
    );

    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioPairResponse_parse_ok()
{
    uint8_t networkId = 1;
    uint8_t gatewayId = 100;
    uint32_t customFrequency = 868000000U;
    const char *encryptKey = "sampleencrypt";
    int8_t rssi = -40;
    
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioPairResponse::iType);
    buffer.appendInt(Statics_RadioPairResponse::iDirection);
    buffer.appendInt(static_cast<uint8_t>(RadioPairResponse::Result::Success));
    buffer.appendInt(gatewayId);
    buffer.appendInt(networkId);
    buffer.appendInt(customFrequency);
    buffer.appendText(encryptKey);
    buffer.appendInt(rssi, true);

    RadioPairResponse msg(buffer);
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioPairResponse::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioPairResponse::eDirection, msg.direction());
    TEST_ASSERT_EQUAL(msg.getResult(), static_cast<uint8_t>(RadioResponse::Result::Success));
    TEST_ASSERT_EQUAL(networkId, msg.model().networkId);
    TEST_ASSERT_EQUAL(gatewayId, msg.model().gatewayId);
    TEST_ASSERT_EQUAL(customFrequency, msg.model().customFrequency);
    TEST_ASSERT_EQUAL(networkId, msg.model().networkId);
    TEST_ASSERT_EQUAL(rssi, msg.model().rssi);

}

void test_RadioPairResponse_parse_error()
{
    // MessageBuffer buffer;
    buffer.appendInt(Statics_RadioPairResponse::iType);
    buffer.appendInt(Statics_RadioPairResponse::iDirection);
    buffer.appendInt(static_cast<uint8_t>(RadioResponse::Result::Success));

    RadioPairResponse msg(buffer);
    TEST_ASSERT_FALSE(msg.parse());
}

void run_tests_RadioPairResponse() {
    RUN_TEST(test_RadioPairResponse_build_resultOk);
    RUN_TEST(test_RadioPairResponse_build_resultError);
    RUN_TEST(test_RadioPairResponse_parse_ok);
    RUN_TEST(test_RadioPairResponse_parse_error);
}