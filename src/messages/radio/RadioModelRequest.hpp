#pragma once

#include "RadioRequest.hpp"
#include "../ModelMessage.hpp"

template<class T>
class RadioModelRequest : public RadioRequest, public ModelMessage<T>
{
public:
    RadioModelRequest(RadioMessageType type, MessageBuffer &buffer)
        :   RadioRequest( type, buffer)
    {}
    virtual ~RadioModelRequest(){}

    virtual bool parse() override
    {
        if(!RadioRequest::parse()){ return false; }
        return parseModel();
    }

    virtual bool build() override
    {
        if(!RadioRequest::build()){ return false; }
        return buildModel();
    }

};
    