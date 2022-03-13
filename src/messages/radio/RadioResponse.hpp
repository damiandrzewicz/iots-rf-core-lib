#pragma once

#include "RadioMessage.hpp"


class RadioResponse : public RadioMessage
{
public:
    enum class Result
    {
        Success,
        Error,
    };

    RadioResponse(RadioMessageType type, MessageBuffer &buffer)
        :   RadioMessage({ type, MessageDirection::Request }, buffer)
    {}
    virtual ~RadioResponse(){}

    virtual bool parse() override
    {
        // Parse base message
        if(!RadioMessage::parse()){ return false; }

        // Check if error returned
        if(!checkResult()){ return false; }

        // Parsing fine
        return true;    
    }

    virtual bool build() override
    {
        // Build base message
        if(!RadioMessage::build()){ return false; }

        if(!buildResult()){ return false; }
        
        // Building fine
        return true;
    }

    bool isErrorResult()
    {
        return result_ != Result::Success;
    }

    void setGenericErrorResult()
    {
        result_ = Result::Error;
    }

    void setDetailedErrorResult(Result result)
    {
        result_ = result;
    }

protected:
    bool checkResult()
    {
        auto sResult = strtok(NULL, delimeter_);
        if(!sResult){ return false; }
        result_ = static_cast<Result>(atoi(sResult));
        return true;
    }

    bool buildResult()
    {
        buffer_.appendDelimeter();
        buffer_.appendInt(static_cast<uint8_t>(result_), true);
        return true;
    }

protected:
    Result result_ = Result::Success;
};
    