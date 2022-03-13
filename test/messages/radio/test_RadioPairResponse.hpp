#pragma once
#include "unity.h"
#include "messages/radio/concrete/RadioPairResponse.hpp"

namespace Statics_RadioPairResponse
{
    static const uint8_t iType = static_cast<uint8_t>(RadioMessageType::Pair);
    static const uint8_t iDirection = static_cast<uint8_t>(MessageDirection::Request);
    static const auto eType = RadioMessageType::Pair;
    static const auto eDirection = MessageDirection::Request;
};


void test_RadioPairResponse_build_resultOk()
{
    MessageBuffer buffer;
    RadioPairResponse msg(buffer);

    auto &model = msg.model();
    model.customFrequency = 123321123U;
    model.encryptKey = "test123456";
    model.gatewayId = 11;
    model.networkId = 99;
    model.rssi = -55;

    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d|%d|%d|%u|%s|%d",
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
    MessageBuffer buffer;
    RadioPairResponse msg(buffer);
    msg.setGenericErrorResult();
    
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d",
        Statics_RadioPairResponse::iType,
        Statics_RadioPairResponse::iDirection,
        static_cast<uint8_t>(RadioPairResponse::Result::Error)
    );

    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void run_tests_RadioPairResponse() {
    RUN_TEST(test_RadioPairResponse_build_resultOk);
    RUN_TEST(test_RadioPairResponse_build_resultError);
}