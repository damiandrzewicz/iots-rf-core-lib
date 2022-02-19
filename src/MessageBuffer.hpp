#pragma once

#include <Arduino.h>
#include <array>

class MessageBuffer
{
public:
    MessageBuffer()
    {
        buffer_.fill('\0');
    }

    void appendText(const char *data)
    {
        strcat(buffer_.data(), data);
    }

    void appendInt(long value)
    {
        snprintf(buffer_.data() + strlen(buffer_.data()), buffer_.size() - strlen(buffer_.data()), "%ld", value);
    }

    void appendFloat(double value, uint8_t size = 4, uint8_t prec = 2)
    {
        char temp[10];
        dtostrf(value, size, prec, temp);
        strcat(buffer_.data(), temp);
    }

    void appendDelimeter()
    {
        strcat(buffer_.data(), "|");
    }

    std::array<char, 100> *buffer()
    {
        return &buffer_;
    }

private:
    std::array<char, 100> buffer_;
};