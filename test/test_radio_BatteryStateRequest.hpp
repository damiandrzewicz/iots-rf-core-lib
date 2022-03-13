// #pragma once
// #include "unity.h"
// #include "message/radio/BatteryStateRequest.hpp"

// using namespace Message;
// using namespace Message::Radio;

// void test_BatteryStateRequest_build()
// {
//     MessageBuffer buffer;

//     uint8_t percentValue = 12;

//     BatteryStateRequestModel model;
//     model.batteryPercent = percentValue;

//     BatteryStateRequest<> bm;
//     TEST_ASSERT_TRUE(bm.build(model, buffer));

//     char temp[50];
//     sprintf(temp, "%d|%d|%d", static_cast<int>(Type::BatteryState), static_cast<int>(Direction::Request), percentValue);
//     TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
// }

// void test_BatteryStateRequest_parse_ok()
// {
//     MessageBuffer buffer;
//     buffer.appendInt(1);
//     buffer.appendDelimeter();
//     buffer.appendInt(1);
//     buffer.appendDelimeter();
//     buffer.appendInt(99);

//     BatteryStateRequestModel model;
//     BatteryStateRequest<> bm;

//     TEST_ASSERT_TRUE(bm.parse(buffer, model));
//     TEST_ASSERT_EQUAL(99, model.batteryPercent);
// }

// void test_BatteryStateRequest_parse_failed()
// {
//     MessageBuffer buffer;
//     buffer.appendInt(1);
//     buffer.appendText("|");

//     BatteryStateRequestModel model;
//     BatteryStateRequest<> bm;

//      TEST_ASSERT_FALSE(bm.parse(buffer, model));
// }

// void run_tests_radio_BatteryStateRequest() {
//     RUN_TEST(test_BatteryStateRequest_build);
//     RUN_TEST(test_BatteryStateRequest_parse_ok);
//     RUN_TEST(test_BatteryStateRequest_parse_failed);
// }