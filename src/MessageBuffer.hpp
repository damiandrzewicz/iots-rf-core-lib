#pragma once

#include <Arduino.h>
#include <array>

class MessageBuffer
{
public:
    MessageBuffer()
    {
        clear();
    }

    MessageBuffer(const char *str)
    {
        clear();
        strncpy(buffer_.data(), str, buffer_.size());
    }

    void appendText(const char *data)
    {
        strcat(buffer_.data(), data);
    }

    void appendInt(long value)
    {
        snprintf(buffer_.data() + strlen(buffer_.data()), buffer_.size() - strlen(buffer_.data()), "%ld", value);
    }

    void appendUint32(uint32_t value)
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

    const char *getDelimeter() const
    {
        return "|";
    }

    void clear()
    {
        buffer_.fill('\0');
    }

    std::array<char, 100> *buffer()
    {
        return &buffer_;
    }

    const std::array<char, 100> *buffer() const
    {
        return &buffer_;
    }

    MessageBuffer &operator=(const char *str)
    {
        clear();
        strncpy(buffer_.data(), str, buffer_.size());
        return *this;
    }

private:
    std::array<char, 100> buffer_;
};