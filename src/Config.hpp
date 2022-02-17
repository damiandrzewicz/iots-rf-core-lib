#pragma once

class Config
{
public:
    virtual ~Config() = default;

    virtual void save() = 0;
    virtual void read() = 0;
    virtual void clear() = 0;

    virtual bool isEmpty() = 0;
    virtual void setEmpty() = 0;
    virtual void setDefaults() = 0;
};