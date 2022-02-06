/*
 * MockEEPROM.cpp - Mock EEPROM implementation
 *
 * This file is mostly stolen from the ESP8266 Arduino code implementation by Ivan Grokhotkov.
 * The original implementation can be found on https://github.com/esp8266/Arduino/blob/master/libraries/EEPROM/
 */

#include "MockEEPROM.h"

void MockEEPROMClass::begin(size_t size) {
    if (size <= 0){
        return;
    }

    if(_data && size != _size) {
        delete[] _data;
        _data = new uint8_t[size];
    } else if(!_data) {
        _data = new uint8_t[size];
    }

    _size = size;
}

void MockEEPROMClass::end() {
    if(!_size)
        return;
    if(_data) {
        delete[] _data;
    }
    _data = 0;
    _size=0;
}

uint8_t MockEEPROMClass::read(int const address) {
    if (address < 0 || (size_t)address >= _size)
        return 0;

    if (!_data)
        return 0;

    return _data[address];
}

void MockEEPROMClass::write(int const address, uint8_t const value) {
    if (address < 0 || (size_t)address >= _size)
        return;

    if (!_data)
        return;

    uint8_t* pData = &_data[address];
    if(*pData != value) {
        *pData = value;
    }
}

uint8_t * MockEEPROMClass::getDataPtr() {
    return &_data[0];
}

uint8_t const * MockEEPROMClass::getConstDataPtr() const {
    return &_data[0];
}

MockEEPROMClass EEPROM;
