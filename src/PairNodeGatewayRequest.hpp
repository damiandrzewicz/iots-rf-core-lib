#pragma once

#include <ArduinoLog.h>
#include "Message.hpp"

struct PairNodeGatewayRequestModel
{

};

class PairNodeGatewayRequest : public Message<PairNodeGatewayRequestModel>
{
public:
    virtual bool parse(const char *str, PairNodeGatewayRequestModel &model) override
    {
        Log.verboseln(F("PairNodeGatewayRequest::parse"));
        return false;
    }

    virtual bool build(const PairNodeGatewayRequestModel &model, char *str) override
    {
        Log.verboseln(F("PairNodeGatewayRequest::build"));
        return false;
    }

private:

};