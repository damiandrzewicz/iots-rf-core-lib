#pragma once
#include "unity.h"
#include "MessageBuffer.hpp"

void test_MessageBuffer_size()
{
    MessageBuffer mb;
    TEST_ASSERT_EQUAL(100, mb.buffer()->size());
}

void test_MessageBuffer_appendInt()
{
    MessageBuffer mb;
    mb.appendInt(123);
    TEST_ASSERT_EQUAL_STRING("123", mb.buffer()->data());
}

void test_MessageBuffer_appendFloat()
{
    MessageBuffer mb;
    mb.appendFloat(123.1234);
    TEST_ASSERT_EQUAL_STRING("123.12", mb.buffer()->data());
}

void test_MessageBuffer_appendFloat_prec1()
{
    MessageBuffer mb;
    mb.appendFloat(12345.6, 2,2);
    TEST_ASSERT_EQUAL_STRING("12345.60", mb.buffer()->data());
}

void test_MessageBuffer_appendFloat_prec2()
{
    MessageBuffer mb;
    mb.appendFloat(12.1528, 2,3);
    TEST_ASSERT_EQUAL_STRING("12.153", mb.buffer()->data());
}

void test_MessageBuffer_appendFloat_appendText()
{
    MessageBuffer mb;
    mb.appendText("some text");
    TEST_ASSERT_EQUAL_STRING("some text", mb.buffer()->data());
}

void test_MessageBuffer_appendFloat_appendDelimeter()
{
    MessageBuffer mb;
    mb.appendDelimeter();
    TEST_ASSERT_EQUAL_STRING("|", mb.buffer()->data());
}

void test_MessageBuffer_appendFloat_appendComplex()
{
    MessageBuffer mb;
    mb.appendInt(123);
    mb.appendDelimeter();
    mb.appendFloat(123.1234);
    mb.appendDelimeter();
    mb.appendText("some text");
    TEST_ASSERT_EQUAL_STRING("123|123.12|some text", mb.buffer()->data());
}

void run_tests_MessageBuffer() {
    RUN_TEST(test_MessageBuffer_size);
    RUN_TEST(test_MessageBuffer_appendInt);
    RUN_TEST(test_MessageBuffer_appendFloat);
    RUN_TEST(test_MessageBuffer_appendFloat_prec1);
    RUN_TEST(test_MessageBuffer_appendFloat_prec2);
    RUN_TEST(test_MessageBuffer_appendFloat_appendText);
    RUN_TEST(test_MessageBuffer_appendFloat_appendDelimeter);
    RUN_TEST(test_MessageBuffer_appendFloat_appendComplex);
}