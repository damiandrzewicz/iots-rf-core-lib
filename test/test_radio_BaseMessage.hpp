// #pragma once
// #include "unity.h"
// #include "message/radio/BaseMessage.hpp"

// using namespace Message;
// using namespace Message::Radio;

// void test_BaseMessage_buildBase()
// {
//     MessageBuffer buffer;

//     BaseMessageModel bmm;
//     bmm.type_ = Type::Pair;
//     bmm.direction_ = Direction::Request;

//     BaseMessage bm { Type::Pair, Direction::Request };
//     TEST_ASSERT_TRUE(bm.buildBase(buffer));

//     char temp[50];
//     sprintf(temp, "%d|%d", static_cast<int>(bmm.type_), static_cast<int>(bmm.direction_));

//     TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
// }

// void test_BaseMessage_parseBase_ok()
// {
//     MessageBuffer buffer;
//     buffer.appendInt(1);
//     buffer.appendText("|");
//     buffer.appendInt(1);

//     BaseMessage bm;

//     TEST_ASSERT_TRUE(bm.parseBase(buffer));
//     TEST_ASSERT_EQUAL(Message::Radio::Type::Pair, bm.getType());
//     TEST_ASSERT_EQUAL(Message::Direction::Request, bm.getDirection());
// }

// void test_BaseMessage_parseBase_failed1()
// {
//     MessageBuffer buffer;
//     buffer.appendInt(1);
//     buffer.appendText("|");

//     BaseMessage bm;

//     TEST_ASSERT_FALSE(bm.parseBase(buffer));
// }

// void test_BaseMessage_parseBase_failed2()
// {
//     MessageBuffer buffer;

//     BaseMessage bm;

//     TEST_ASSERT_FALSE(bm.parseBase(buffer));
// }

// void test_BaseMessage_parseBase_failed3()
// {
//     MessageBuffer buffer;
//     buffer.appendText("test");

//     BaseMessage bm;

//     TEST_ASSERT_FALSE(bm.parseBase(buffer));
// }

// void run_tests_radio_BaseMessage() {
//     RUN_TEST(test_BaseMessage_buildBase);
//     RUN_TEST(test_BaseMessage_parseBase_ok);
//     RUN_TEST(test_BaseMessage_parseBase_failed1);
//     RUN_TEST(test_BaseMessage_parseBase_failed2);
//     RUN_TEST(test_BaseMessage_parseBase_failed3);
// }