#pragma once

#include "MessageBuffer.hpp"

template<class T>
class IMessage
{
public:
    virtual bool parse(MessageBuffer &buffer, T &model) = 0;
    virtual bool build(const T &model, MessageBuffer &buffer) = 0;
};