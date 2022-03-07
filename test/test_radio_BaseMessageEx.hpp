#pragma once
#include "unity.h"
#include "message/radio/BaseMessageEx.hpp"

using namespace Message;
using namespace Message::Radio;

void test_BaseMessageEx_build()
{
    MessageBuffer buffer;

    BaseMessageExModel bmm;
    bmm.type_ = Type::Pair;
    bmm.direction_ = Direction::Request;
    bmm.batteryPercent = 12;

    BaseMessageEx<> bm;
    TEST_ASSERT_TRUE(bm.build(bmm, buffer));
    TEST_ASSERT_EQUAL_STRING("1|1|12", buffer.buffer()->data());
}

void test_BaseMessageEx_parseBase_ok()
{
    MessageBuffer buffer;
    buffer.appendInt(1);
    buffer.appendDelimeter();
    buffer.appendInt(1);
    buffer.appendDelimeter();
    buffer.appendInt(99);

    BaseMessageExModel bmm;
    BaseMessageEx<> bm;

    TEST_ASSERT_TRUE(bm.parse(buffer, bmm));
    TEST_ASSERT_EQUAL(Message::Radio::Type::Pair, bmm.type_);
    TEST_ASSERT_EQUAL(Message::Direction::Request, bmm.direction_);
    TEST_ASSERT_EQUAL(99, bmm.batteryPercent);
}

void test_BaseMessageEx_parseBase_failed()
{
    MessageBuffer buffer;
    buffer.appendInt(1);
    buffer.appendText("|");

    BaseMessageExModel bmm;
    BaseMessageEx<> bm;

    TEST_ASSERT_FALSE(bm.parse(buffer, bmm));
}

void run_tests_radio_BaseMessageEx() {
    RUN_TEST(test_BaseMessageEx_build);
    RUN_TEST(test_BaseMessageEx_parseBase_ok);
    RUN_TEST(test_BaseMessageEx_parseBase_failed);
}