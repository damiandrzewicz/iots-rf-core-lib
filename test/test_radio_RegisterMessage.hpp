// #pragma once
// #include "unity.h"
// #include "message/radio/RegisterMessage.hpp"

// using namespace Message;
// using namespace Message::Radio;

// void test_RegisterMessage_build()
// {
//     MessageBuffer buffer;

//     RegisterMessageModel model;
//     model.type_ = Type::Pair;
//     model.direction_ = Direction::Request;
//     model.batteryPercent = 12;
//     model.uuid = "D27AE0D1-F20A-44DC-96F6-8A3F5A273EBD";

//     RegisterMessage msg;
//     TEST_ASSERT_TRUE(msg.build(model, buffer));
//     TEST_ASSERT_EQUAL_STRING("1|1|12|D27AE0D1-F20A-44DC-96F6-8A3F5A273EBD", buffer.buffer()->data());
// }

// void test_RegisterMessage_parse_ok()
// {
//     MessageBuffer buffer;
//     buffer.appendInt(1);
//     buffer.appendDelimeter();
//     buffer.appendInt(1);
//     buffer.appendDelimeter();
//     buffer.appendInt(99);
//     buffer.appendDelimeter();
//     buffer.appendText("D27AE0D1-F20A-44DC-96F6-8A3F5A273EBD");


//     RegisterMessageModel model;
//     RegisterMessage msg;

//     TEST_ASSERT_TRUE(msg.parse(buffer, model));
//     TEST_ASSERT_EQUAL(Message::Radio::Type::Pair, model.type_);
//     TEST_ASSERT_EQUAL(Message::Direction::Request, model.direction_);
//     TEST_ASSERT_EQUAL(99, model.batteryPercent);
//     TEST_ASSERT_EQUAL_STRING("D27AE0D1-F20A-44DC-96F6-8A3F5A273EBD", model.uuid);
// }

// void test_RegisterMessage_parse_failed()
// {
//     MessageBuffer buffer;
//     buffer.appendInt(1);
//     buffer.appendText("|");

//     RegisterMessageModel model;
//     RegisterMessage msg;

//     TEST_ASSERT_FALSE(msg.parse(buffer, model));
// }

// void run_tests_radio_RegisterMessage() {
//     RUN_TEST(test_RegisterMessage_build);
//     RUN_TEST(test_RegisterMessage_parse_ok);
//     RUN_TEST(test_RegisterMessage_parse_failed);
// }