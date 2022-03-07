#pragma once
#include "../IMessage.hpp"
#include "../MessageType.hpp"
#include "BaseMessage.hpp"

namespace Message
{
    namespace Radio
    {
        struct BaseMessageExModel : public BaseMessageModel
        {
            int8_t batteryPercent = -1;
        };

        template<class T = BaseMessageExModel>
        class BaseMessageEx : public BaseMessage<T>
        {
        public:
            bool parse(MessageBuffer &buffer, T &model)
            {   
                if(!BaseMessage<T>::parse(buffer, model))
                {
                    return false;
                }

                auto delim = buffer.getDelimeter();
                auto batteryPercent = strtok(NULL, delim);

                if(!batteryPercent)
                {
                    return false;
                }

                model.batteryPercent = atoi(batteryPercent);

                return true;
            }

            bool build(const T &model, MessageBuffer &buffer)
            {
                if(!BaseMessage<T>::build(model, buffer))
                {
                    return false;
                }

                buffer.appendDelimeter();
                buffer.appendInt(model.batteryPercent);

                return true;
            }
        };
    };
};