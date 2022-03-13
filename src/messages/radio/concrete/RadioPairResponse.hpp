#pragma once

#include "../RadioModelResponse.hpp"

struct RadioPairResponseModel final
{
    uint8_t networkId;
    uint8_t gatewayId;
    uint32_t customFrequency;
    const char *encryptKey;
    int8_t rssi;
};

class RadioPairResponse final : public RadioModelResponse<RadioPairResponseModel>
{
public:
    RadioPairResponse(MessageBuffer &buffer)
        : RadioModelResponse<RadioPairResponseModel>(RadioMessageType::Pair, buffer)
    {}

    virtual bool parseModel() override
    { 
        auto sGatewayId = strtok(NULL, delimeter_);
        auto sNetworkId = strtok(NULL, delimeter_);
        auto sCustomFrequency = strtok(NULL, delimeter_);
        auto sEncryptKey = strtok(NULL, delimeter_);
        auto sRssi = strtok(NULL, delimeter_);

        if(!sGatewayId || !sNetworkId || !sCustomFrequency || !sEncryptKey || !sRssi){ return false; }

        model().gatewayId = atoi(sGatewayId);
        model().networkId = atoi(sNetworkId);
        model().customFrequency = atol(sCustomFrequency);
        model().encryptKey = sEncryptKey;
        model().rssi = atoi(sRssi);

        return true;
    }

    virtual bool buildModel() override
    {
        buffer_.appendDelimeter();
        buffer_.appendInt(model().gatewayId);
        buffer_.appendInt(model().networkId);
        buffer_.appendInt(model().customFrequency);
        buffer_.appendText(model().encryptKey);
        buffer_.appendInt(model().rssi, true);
        return true;
    }
private:

};