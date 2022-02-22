#pragma once

template<class T>
class Message
{
public:
    virtual bool parse(const char *str, T &model) = 0;
    virtual bool build(const T &model, char *str) = 0;
};