/*
 * MockEEPROM.cpp - Mock EEPROM implementation
 *
 * This file is mostly stolen from the ESP8266 Arduino core implementation by Ivan Grokhotkov.
 * The original implementation can be found on https://github.com/esp8266/Arduino/blob/master/libraries/EEPROM/
 */

#ifndef MockEEPROM_h

#define MockEEPROM_h
#include <ArduinoFake.h>

class MockEEPROMClass {
    public:
        void begin(size_t size);
        uint8_t read(int const address);
        void write(int const address, uint8_t const val);
        void end();

        uint8_t * getDataPtr();
        uint8_t const * getConstDataPtr() const;

        template<typename T>
        T &get(int const address, T &t) {
            if(address < 0 || address + sizeof(T) > _size)
                return t;

            memcpy((uint8_t*) &t, _data + address, sizeof(T));
            return t;
        }

        template<typename T>
        const T &put(int const address, const T &t) {
            if (address < 0 || address + sizeof(T) > _size)
                return t;
            if (memcmp(_data + address, (const uint8_t*)&t, sizeof(T)) != 0) {
                memcpy(_data + address, (const uint8_t*)&t, sizeof(T));
            }
            
            return t;
        }

        size_t length() {return _size;}

        uint8_t& operator[](int const address) {return getDataPtr()[address];}
        uint8_t const & operator[](int const address) const {return getConstDataPtr()[address];}

    protected:
        uint32_t sector;
        uint8_t* _data;
        size_t _size;

};
#endif
