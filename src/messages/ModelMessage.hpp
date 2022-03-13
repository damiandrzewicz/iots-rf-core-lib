#pragma once

#include <Arduino.h>

template<class T>
class ModelMessage
{
public:
    virtual ~ModelMessage(){}

    T &model(){ return model_; }

    virtual bool parseModel() = 0;
    virtual bool buildModel() = 0;

private:
    T model_;
};
