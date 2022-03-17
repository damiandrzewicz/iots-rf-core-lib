#pragma once

#include "../RadioModelResponse.hpp"

struct RadioBatteryStateResponseModel
{
    int8_t batteryPercentValue;
};

class RadioBatteryStateResponse final : public RadioModelResponse<RadioBatteryStateResponseModel>
{
public:
    RadioBatteryStateResponse(MessageBuffer &buffer)
        : RadioModelResponse<RadioBatteryStateResponseModel>(RadioMessageType::BatteryState, buffer)
    {}

    virtual bool parseModel() override
    { 
        auto sPercentValue = strtok(NULL, delimeter_);
        if(!sPercentValue){ return false; }
        model().batteryPercentValue = atoi(sPercentValue);
        return true;
    }

    virtual bool buildModel() override
    {
        buffer_.appendDelimeter();
        buffer_.appendInt(model().batteryPercentValue, true);
        return true;
    }
private:

};