#pragma once

#include "../RadioModelResponse.hpp"

struct RadioRegisterResponseModel final
{
    int8_t nodeId;
};

class RadioRegisterResponse final : public RadioModelResponse<RadioRegisterResponseModel>
{
public:
    RadioRegisterResponse(MessageBuffer &buffer)
        : RadioModelResponse<RadioRegisterResponseModel>(RadioMessageType::Register, buffer)
    {}

    virtual bool parseModel() override
    { 
        auto sNodeId = strtok(NULL, delimeter_);
        if(!sNodeId){ return false; }
        model().nodeId = atoi(sNodeId);
        return true;
    }

    virtual bool buildModel() override
    {
        buffer_.appendDelimeter();
        buffer_.appendInt(model().nodeId, true);
        return true;
    }
private:

};