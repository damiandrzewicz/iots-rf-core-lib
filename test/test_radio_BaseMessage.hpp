#pragma once
#include "unity.h"
#include "message/radio/BaseMessage.hpp"

using namespace Message;
using namespace Message::Radio;



void test_BaseMessage_build()
{
    MessageBuffer buffer;

    BaseMessageModel bmm;
    bmm.type_ = Type::Pair;
    bmm.direction_ = Direction::Request;

    BaseMessage<> bm;
    TEST_ASSERT_TRUE(bm.build(bmm, buffer));
    TEST_ASSERT_EQUAL_STRING("1|1", buffer.buffer()->data());
}

void test_BaseMessage_parseBase_ok()
{
    MessageBuffer buffer;
    buffer.appendInt(1);
    buffer.appendText("|");
    buffer.appendInt(1);

    BaseMessageModel bmm;
    BaseMessage<> bm;

    TEST_ASSERT_TRUE(bm.parse(buffer, bmm));
    TEST_ASSERT_EQUAL(Message::Radio::Type::Pair, bmm.type_);
    TEST_ASSERT_EQUAL(Message::Direction::Request, bmm.direction_);
}

void test_BaseMessage_parseBase_failed1()
{
    MessageBuffer buffer;
    buffer.appendInt(1);
    buffer.appendText("|");

    BaseMessageModel bmm;
    BaseMessage<> bm;

    TEST_ASSERT_FALSE(bm.parse(buffer, bmm));
}

void test_BaseMessage_parseBase_failed2()
{
    MessageBuffer buffer;

    BaseMessageModel bmm;
    BaseMessage<> bm;

    TEST_ASSERT_FALSE(bm.parse(buffer, bmm));
}

void test_BaseMessage_parseBase_failed3()
{
    MessageBuffer buffer;
    buffer.appendText("test");

    BaseMessageModel bmm;
    BaseMessage<> bm;

    TEST_ASSERT_FALSE(bm.parse(buffer, bmm));
}

void run_tests_radio_BaseMessage() {
    RUN_TEST(test_BaseMessage_build);
    RUN_TEST(test_BaseMessage_parseBase_ok);
    RUN_TEST(test_BaseMessage_parseBase_failed1);
    RUN_TEST(test_BaseMessage_parseBase_failed2);
    RUN_TEST(test_BaseMessage_parseBase_failed3);
}