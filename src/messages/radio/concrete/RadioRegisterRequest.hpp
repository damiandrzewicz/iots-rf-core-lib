#pragma once

#include "../RadioModelRequest.hpp"

struct RadioRegisterRequestModel final
{
    const char *uuid;
};


class RadioRegisterRequest final : public RadioModelRequest<RadioRegisterRequestModel>
{
public:
    RadioRegisterRequest(MessageBuffer &buffer)
        : RadioModelRequest<RadioRegisterRequestModel>(RadioMessageType::Register, buffer)
        {}

        virtual bool parseModel() override
    {
        auto sUuid = strtok(NULL, delimeter_);
        if(!sUuid){ return false; }
        model().uuid = sUuid;
        return true;
    }

    virtual bool buildModel() override
    {
        buffer_.appendDelimeter();
        buffer_.appendText(model().uuid, true);
        return true;
    }
};

    
