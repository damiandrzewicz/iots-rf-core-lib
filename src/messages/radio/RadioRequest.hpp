#pragma once

#include "RadioMessage.hpp"


class RadioRequest : public RadioMessage
{
public:
    RadioRequest(RadioMessageType type, MessageBuffer &buffer)
        :   RadioMessage({ type, MessageDirection::Request }, buffer)
    {}
    virtual ~RadioRequest(){}

};
    