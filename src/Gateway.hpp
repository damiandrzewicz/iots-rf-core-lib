#pragma once
#include "Appliance.hpp"

class Gateway : public Appliance
{
public:
    Gateway() = default;

    virtual void setup() override;
    virtual void loop() override;
};