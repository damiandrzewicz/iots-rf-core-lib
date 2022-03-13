#pragma once

#include "RadioMessage.hpp"


class RadioResponse : public RadioMessage
{
public:
    enum class Result
    {
        Success,
        Error,
        ParserError
    };

    RadioResponse(RadioMessageType type, MessageBuffer &buffer)
        :   RadioMessage({ type, MessageDirection::Response }, buffer)
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

    bool isError()
    {
        return static_cast<uint8_t>(result_) >= static_cast<uint8_t>(Result::Error);
    }

    void setError(Result result = Result::Error)
    {
        result_ = result;
    }

    Result getResult()
    {
        return result_;
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
    