#pragma once

#include "RadioConfig.hpp"
#include "UUIDConfig.hpp"

class Core
{
public:
    ~Core() = default;

    virtual void setup() = 0;
    virtual void loop() = 0;

    //virtual void configureRadio(const RadioConfigData &data) = 0;

};