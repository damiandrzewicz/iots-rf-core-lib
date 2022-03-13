#pragma once

#include "MessageBuffer.hpp"
#include "../Message.hpp"
#include "RadioMessageType.hpp"
#include "../MessageDirection.hpp"

struct RadioMessageModel
{
    RadioMessageType type_;
    MessageDirection direction_;
};

class RadioMessage : public Message
{
public:
    RadioMessage(RadioMessageModel radioMessageModel, MessageBuffer &buffer) : radioMessageModel_(radioMessageModel), buffer_(buffer)
    {
        delimeter_ = buffer_.getDelimeter();
    }

    virtual bool parse()
    {
        auto sType = strtok(buffer_.buffer()->data(), delimeter_);
        auto sDirection = strtok(NULL, delimeter_);

        if(!sType || !sDirection){ return false; }

        auto eType = static_cast<RadioMessageType>(atoi(sType));
        auto eDirection= static_cast<MessageDirection>(atoi(sDirection));

        if(eType != radioMessageModel_.type_ || eDirection != radioMessageModel_.direction_){ return false; }

        radioMessageModel_.type_ = eType;
        radioMessageModel_.direction_ = eDirection;

        return true;
    }

    virtual bool build()
    {
        buffer_.clear();
        buffer_.appendInt(static_cast<uint8_t>(radioMessageModel_.type_));
        buffer_.appendInt(static_cast<uint8_t>(radioMessageModel_.direction_), true);
        return true;
    }

    auto type(){ return radioMessageModel_.type_; }
    auto direction(){ return radioMessageModel_.direction_; }

protected:
    MessageBuffer &buffer_;
    const char *delimeter_;

private:
    RadioMessageModel radioMessageModel_;
};