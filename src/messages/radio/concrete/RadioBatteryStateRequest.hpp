#pragma once

#include "../RadioRequest.hpp"

class RadioBatteryStateRequest final : public RadioRequest
{
public:
    RadioBatteryStateRequest(MessageBuffer &buffer)
        : RadioRequest(RadioMessageType::BatteryState, buffer)
        {}
};

    
