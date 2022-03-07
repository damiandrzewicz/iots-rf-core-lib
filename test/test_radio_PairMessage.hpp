#pragma once
#include "unity.h"
#include "message/radio/PairMessage.hpp"

using namespace Message;
using namespace Message::Radio;

void test_PairMessage_build()
{
    MessageBuffer buffer;

    PairMessageModel model;
    model.type_ = Type::Pair;
    model.direction_ = Direction::Request;
    model.batteryPercent = 12;
    model.networkId = 123;
    model.gatewayId = 10;
    model.customFrequency = 869000000;
    model.rssi = -55;
    model.encryptKey = "testKey";

    PairMessage msg;
    TEST_ASSERT_TRUE(msg.build(model, buffer));
    TEST_ASSERT_EQUAL_STRING("1|1|12|123|10|869000000|-55|testKey", buffer.buffer()->data());
}

void test_PairMessage_parse_ok()
{
    MessageBuffer buffer;
    buffer.appendInt(1);
    buffer.appendDelimeter();
    buffer.appendInt(1);
    buffer.appendDelimeter();
    buffer.appendInt(99);
    buffer.appendDelimeter();
    buffer.appendInt(123);
    buffer.appendDelimeter();
    buffer.appendInt(10);
    buffer.appendDelimeter();
    buffer.appendInt(869000000);
    buffer.appendDelimeter();
    buffer.appendText("testKey");
    buffer.appendDelimeter();
    buffer.appendInt(-55);


    PairMessageModel model;
    PairMessage msg;

    TEST_ASSERT_TRUE(msg.parse(buffer, model));
    TEST_ASSERT_EQUAL(Message::Radio::Type::Pair, model.type_);
    TEST_ASSERT_EQUAL(Message::Direction::Request, model.direction_);
    TEST_ASSERT_EQUAL(99, model.batteryPercent);
    TEST_ASSERT_EQUAL(123, model.networkId);
    TEST_ASSERT_EQUAL(10, model.gatewayId);
    TEST_ASSERT_EQUAL(869000000, model.customFrequency);
    TEST_ASSERT_EQUAL(-55, model.rssi);
    TEST_ASSERT_EQUAL_STRING("testKey", model.encryptKey);
}

void test_PairMessage_parse_failed()
{
    MessageBuffer buffer;
    buffer.appendInt(1);
    buffer.appendText("|");

    PairMessageModel model;
    PairMessage msg;

    TEST_ASSERT_FALSE(msg.parse(buffer, model));
}

void run_tests_radio_PairMessage() {
    RUN_TEST(test_PairMessage_build);
    RUN_TEST(test_PairMessage_parse_ok);
    RUN_TEST(test_PairMessage_parse_failed);
}