#pragma once

#include <Arduino.h>

class Message
{
public:
    virtual bool parse() = 0;
    virtual bool build() = 0;
};

