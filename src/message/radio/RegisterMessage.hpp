#pragma once

#include "../MessageType.hpp"
#include "BaseMessageEx.hpp"

namespace Message
{
    namespace Radio
    {
        struct RegisterMessageModel final : public BaseMessageExModel
        {
            const char *uuid;
        };

        class RegisterMessage final : public BaseMessageEx<RegisterMessageModel>
        {
        public:
            bool parse(MessageBuffer &buffer, RegisterMessageModel &model) override
            {
                if(!BaseMessageEx<RegisterMessageModel>::parse(buffer, model))
                {
                    return false;
                }

                auto delim = buffer.getDelimeter();
                auto uuid = strtok(NULL, delim);

                if(!uuid)
                {
                    return false;
                }

                model.uuid = uuid;

                return true;
            }

            bool build(const RegisterMessageModel &model, MessageBuffer &buffer) override
            {
                if(!BaseMessageEx<RegisterMessageModel>::build(model, buffer))
                {
                    return false;
                }

                buffer.appendDelimeter();
                buffer.appendText(model.uuid);

                return true;
            }

        private:
        };
    }
}

