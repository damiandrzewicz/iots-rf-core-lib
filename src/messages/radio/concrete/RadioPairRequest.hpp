#pragma once

#include "../RadioRequest.hpp"


class RadioPairRequest final : public RadioRequest
{
public:
    RadioPairRequest(MessageBuffer &buffer)
        : RadioRequest(RadioMessageType::Pair, buffer){}
};

    
