#pragma once
#include "unity.h"
#include "messages/radio/RadioModelResponse.hpp"

namespace Statics_RadioModelResponse
{
    static const auto eType = RadioMessageType::Alive;
    static const auto eDirection = MessageDirection::Response;
    static const uint8_t iType = static_cast<uint8_t>(eType);
    static const uint8_t iDirection = static_cast<uint8_t>(eDirection);

    struct RadioModelResponseModelTest
    {
        uint8_t int_value;
        bool bool_value;
    };

    class RadioModelResponseTest : public RadioModelResponse<RadioModelResponseModelTest>
    {
    public:
        RadioModelResponseTest(RadioMessageType type, MessageBuffer &buffer)
            :   RadioModelResponse<RadioModelResponseModelTest>( type, buffer)
        {}
        virtual ~RadioModelResponseTest(){}

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

void test_RadioModelResponse_build_resultOk()
{
    MessageBuffer buffer;

    Statics_RadioModelResponse::RadioModelResponseTest msg(Statics_RadioModelResponse::eType, buffer);
    msg.model().bool_value = true;
    msg.model().int_value = 123;
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d|%d|%d", 
        Statics_RadioModelResponse::iType, 
        Statics_RadioModelResponse::iDirection, 
        static_cast<uint8_t>(RadioResponse::Result::Success),
        msg.model().int_value, 
        msg.model().bool_value
    );
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioModelResponse_build_resultError()
{
    MessageBuffer buffer;

    Statics_RadioModelResponse::RadioModelResponseTest msg(Statics_RadioModelResponse::eType, buffer);
    msg.setError();
    TEST_ASSERT_TRUE(msg.build());

    char temp[50];
    sprintf(temp, "%d|%d|%d", 
        Statics_RadioModelResponse::iType, 
        Statics_RadioModelResponse::iDirection, 
        static_cast<uint8_t>(RadioResponse::Result::Error)
    );
    TEST_ASSERT_EQUAL_STRING(temp, buffer.buffer()->data());
}

void test_RadioModelResponse_parse_resultOk()
{
    MessageBuffer buffer;
    buffer.appendInt(Statics_RadioModelResponse::iType);
    buffer.appendInt(Statics_RadioModelResponse::iDirection);
    buffer.appendInt(static_cast<uint8_t>(RadioResponse::Result::Success));
    buffer.appendInt(123);
    buffer.appendInt(1, true);
    

    Statics_RadioModelResponse::RadioModelResponseTest msg(Statics_RadioModelResponse::eType, buffer); 
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_EQUAL(Statics_RadioModelResponse::eType, msg.type());
    TEST_ASSERT_EQUAL(Statics_RadioModelResponse::eDirection, msg.direction());
    TEST_ASSERT_EQUAL(123, msg.model().int_value);
    TEST_ASSERT_EQUAL(1, msg.model().bool_value);
}

void test_RadioModelResponse_parse_resultError()
{
    MessageBuffer buffer;
    buffer.appendInt(Statics_RadioModelResponse::iType);
    buffer.appendInt(Statics_RadioModelResponse::iDirection);
    buffer.appendInt(static_cast<uint8_t>(RadioResponse::Result::Error), true);

    Statics_RadioModelResponse::RadioModelResponseTest msg(Statics_RadioModelResponse::eType, buffer); 
    TEST_ASSERT_TRUE(msg.parse());
    TEST_ASSERT_TRUE(msg.isError());
    TEST_ASSERT_EQUAL(static_cast<uint8_t>(RadioResponse::Result::Error), msg.getResult());
}

void test_RadioModelResponse_parse_error()
{
    MessageBuffer buffer;
    buffer.appendInt(Statics_RadioModelResponse::iType);

    Statics_RadioModelResponse::RadioModelResponseTest msg(Statics_RadioModelResponse::eType, buffer); 
    TEST_ASSERT_FALSE(msg.parse());
}

void run_tests_RadioModelResponse() {
    RUN_TEST(test_RadioModelResponse_build_resultOk);
    RUN_TEST(test_RadioModelResponse_build_resultError);
    RUN_TEST(test_RadioModelResponse_parse_resultOk);
    RUN_TEST(test_RadioModelResponse_parse_resultError);
    RUN_TEST(test_RadioModelResponse_parse_error);
}