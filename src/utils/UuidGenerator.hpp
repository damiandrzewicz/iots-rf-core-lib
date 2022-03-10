#pragma once

#include <Arduino.h>
#include <TrueRandom.h>

class UuidGenerator
{
public:
    void generateUuid(char *buffer, size_t size)
    {
        byte uuidNumber[16];
        TrueRandom.uuid(uuidNumber);
        printUuid(uuidNumber, buffer, size);
    }

    void printUuid(byte* uuidNumber, char *buffer, size_t size) {
        int i;
        for (i=0; i<16; i++) {
            if (i==4 || i ==6 || i==8 || i==10)
            {
                appendchar(buffer, size, '-');
            }
            printHex(uuidNumber[i], buffer, size);
        }
    }

    void printHex(byte number, char *buffer, size_t size) {
        int topDigit = number >> 4;
        int bottomDigit = number & 0x0f;
        // Print high hex digit
        appendchar(buffer, size, letters[topDigit]);
        // Low hex digit
        appendchar(buffer, size, letters[bottomDigit]);
    }

    char *appendchar(char *szString, size_t strsize, char c)
    {
        size_t len = strlen(szString);
        if((len+1) < strsize)
        {
            szString[len++] = c;
            szString[len] = '\0';
            return szString;
        }
        return NULL;
    }

private:
    char letters[17] = "0123456789ABCDEF";
    
};