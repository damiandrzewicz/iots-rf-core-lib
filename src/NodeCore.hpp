#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "RFM69_ATC.h"
#include "RadioConfig.hpp"
#include "UUIDConfig.hpp"

#define FLASH_SS 8

class NodeCore{
public:

    NodeCore()
        :   radioConfig_(0),
            uuidConfig_(radioConfig_.dataSize() + 1)
    {}

    void test();

private:
    RFM69_ATC radio_;
    RadioConfig radioConfig_;
    UUIDConfig uuidConfig_;
};