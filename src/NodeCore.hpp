#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "RFM69_ATC.h"

#define FLASH_SS 8

class NodeCore{
public:
    void test();

private:
    RFM69_ATC radio_;
};