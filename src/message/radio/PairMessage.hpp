#pragma once

#include "../MessageType.hpp"
#include "BaseMessageEx.hpp"

namespace Message
{
    namespace Radio
    {
        struct PairMessageModel final : public BaseMessageExModel
        {
            uint8_t networkId;
            uint8_t gatewayId;
            unsigned long customFrequency;
            const char *encryptKey;
            int8_t rssi;
        };

        class PairMessage final : public BaseMessageEx<PairMessageModel>
        {
        public:
            bool parse(MessageBuffer &buffer, PairMessageModel &model)
            {
                if(!BaseMessageEx<PairMessageModel>::parse(buffer, model))
                {
                    return false;
                }

                auto delim = buffer.getDelimeter();
                auto networkId = strtok(NULL, delim);
                auto gatewayId = strtok(NULL, delim);
                auto customFrequency = strtok(NULL, delim);
                auto encryptKey = strtok(NULL, delim);
                auto rssi = strtok(NULL, delim);

                if(!networkId || !gatewayId || !customFrequency || !encryptKey || !rssi)
                {
                    return false;
                }

                model.networkId = atoi(networkId);
                model.gatewayId = atoi(gatewayId);
                model.customFrequency = atoi(customFrequency);
                model.encryptKey = encryptKey;
                model.rssi = atoi(rssi);

                return true;
            }

            bool build(const PairMessageModel &model, MessageBuffer &buffer)
            {
                if(!BaseMessageEx<PairMessageModel>::build(model, buffer))
                {
                    return false;
                }

                buffer.appendDelimeter();
                buffer.appendInt(model.networkId);
                buffer.appendDelimeter();
                buffer.appendInt(model.gatewayId);
                buffer.appendDelimeter();
                buffer.appendInt(model.customFrequency);
                buffer.appendDelimeter();
                buffer.appendInt(model.rssi);
                buffer.appendDelimeter();
                buffer.appendText(model.encryptKey);


                return true;
            }

        private:

        };

    };
};