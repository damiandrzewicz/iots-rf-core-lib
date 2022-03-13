#pragma once
#include "unity.h"
#include "messages/radio/RadioModelRequest.hpp"

namespace Statics_RadioModelRequest
{
    static const auto eType = RadioMessageType::Alive;
    static const auto eDirection = MessageDirection::Request;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);

    struct RadioModelRequestModelTest
    {
        uint8_t int_value;
        bool bool_value;
    };

    class RadioModelRequestTest : public RadioModelRequest<RadioModelRequestModelTest>
    {
    public:
        RadioModelRequestTest(RadioMessageType type, MessageBuffer &buffer)
            :   RadioModelRequest<RadioModelRequestModelTest>( type, buffer)
        {}
        virtual ~RadioModelRequestTest(){}

        virtual bool parseModel() override
        {
            auto sInt_value = strtok(NULL, delimeter_);
            auto sBool_value = strtok(NULL, delimeter_);

            if(!sInt_value || !sInt_value){ return false; }

            model().int_value = atoi(sInt_value);
            model().bool_value = atoi(sBool_value);

            return true;
        }

        virtual bool buildModel() override
        {
            buffer_.appendDelimeter();
            buffer_.appendInt(model().int_value);
            buffer_.appendInt(model().bool_value, true);
            return true;
        }
    };
};

void test_RadioModelRequest_build()
{
    MessageBuffer buffer;

    Statics_RadioModelRequest::RadioModelRequestTest msg(Statics_RadioModelRequest::eType, buffer);
    msg.model().bool_value = true;
    msg.model().int_value = 123;
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d|%d", 
        Statics_RadioModelRequest::iType, 
        Statics_RadioModelRequest::iDirection, 
        msg.model().int_value, 
        msg.model().bool_value
    );
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioModelRequest_parse_ok()
{
    MessageBuffer buffer;
    buffer.appendInt(Statics_RadioModelRequest::iType);
    buffer.appendInt(Statics_RadioModelRequest::iDirection);
    buffer.appendInt(123);
    buffer.appendInt(true);
    

    Statics_RadioModelRequest::RadioModelRequestTest msg(Statics_RadioModelRequest::eType, buffer);    
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioModelRequest::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioModelRequest::eDirection, msg.direction());
    TEST_ASSERT_EQUAL(123, msg.model().int_value);
    TEST_ASSERT_EQUAL(true, msg.model().bool_value);
}

void test_RadioModelRequest_parse_error()
{
    MessageBuffer buffer;
    buffer.appendInt(Statics_RadioModelRequest::iDirection, true);

    Statics_RadioModelRequest::RadioModelRequestTest msg(Statics_RadioModelRequest::eType, buffer);
    TEST_ASSERT_FALSE(msg.parse());
}


void run_tests_RadioModelRequest() {
    RUN_TEST(test_RadioModelRequest_build);
    RUN_TEST(test_RadioModelRequest_parse_ok);
    RUN_TEST(test_RadioModelRequest_parse_error);
}