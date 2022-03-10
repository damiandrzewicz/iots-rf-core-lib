#pragma once
#include "../IMessage.hpp"
#include "../MessageType.hpp"

namespace Message
{
    namespace Radio
    {
        struct BaseMessageModel
        {
            Message::Radio::Type type_;
            Message::Direction direction_;
        };

        template<class T = BaseMessageModel>
        class BaseMessage : public IMessage<T>
        {
        public:
            bool parse(MessageBuffer &buffer, T &model) override
            {   
                auto delim = buffer.getDelimeter();
                auto type = strtok(buffer.buffer()->data(), delim);
                auto direction = strtok(NULL, delim);

                if(!type || !direction)
                {
                    return false;
                }

                model.type_ = static_cast<Message::Radio::Type>(atoi(type));
                model.direction_ = static_cast<Message::Direction>(atoi(direction));

                return true;
            }

            bool build(const T &model, MessageBuffer &buffer) override
            {
                buffer.clear();
                buffer.appendInt(static_cast<uint8_t>(model.type_));
                buffer.appendDelimeter();
                buffer.appendInt(static_cast<uint8_t>(model.direction_));
                return true;
            }
        };
    };
};