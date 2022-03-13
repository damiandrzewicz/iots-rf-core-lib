#pragma once

#include "RadioResponse.hpp"
#include "../ModelMessage.hpp"

template<class T>
class RadioModelResponse : public RadioResponse, public ModelMessage<T>
{
public:
    RadioModelResponse(RadioMessageType type, MessageBuffer &buffer)
        :   RadioResponse( type, buffer)
    {}
    virtual ~RadioModelResponse(){}

    virtual bool parse() override
    {
        if(!RadioResponse::parse()){ return false; }
        return !isError() ? this->parseModel() : true;
    }

    virtual bool build() override
    {
        if(!RadioResponse::build()){ return false; }
        return !isError() ? this->buildModel() : true;
    }

};
    