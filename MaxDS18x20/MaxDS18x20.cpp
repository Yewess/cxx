#ifdef __AVR__
    #include <stdint.h>
    #include <Arduino.h>
    #include <HardwareSerial.h>
    #include <OneWire.h>
#else
    #include <cstring>
    #include <cstddef>
    #include <cstdint>
    #include <FakeOneWire.h>
#endif //__AVR__

#include "MaxDS18x20.h"

uint16_t MaxMem::getTemp(const MaxRom& max_rom) {
    uint16_t x10degrees = mem.temperature;
    if (max_rom.maxRomU.isTypeS()) {
        x10degrees = x10degrees << 3; // 9 bit resolution default
        if (reserved3 == 0x10)
            // "count remain" gives full 12 bit resolution
            return (x10degrees & 0xFFF0) + 12 - reserved2;
    } else {
        switch (getResolution()) {
            case  9:  // 9 bit resolution
                return x10degrees & ~7;
            case 10:  // 10 bit resolution
                return x10degrees & ~3;
            case 11:  // 11 bit resulution
                return x10degrees & ~1;
            default:  // 12 bit resolution
                return x10degrees;
        }
    }
}

uint8_t MaxDS18x20::startConversion(void) {
    tempCache = errorTemp;
    if (oneWire.reset()) {
        oneWire.write(Ds18x20Command::skipRom);
        oneWire.write(Ds18x20Command::convertT);
        if (!conversionDone())
            status = MaxStatus::converting;
    } else
        status = busError;
    return status;
}

bool MaxDS18x20::conversionDone(void) {
    tempCache = errorTemp;
    if (status == MaxStatus::converting)
        if (oneWire.read_bit() == 1) {
            status = MaxStatus::complete;
            return true;
        } else
            return false;
    else
        return true;
}

void MaxDS18x20::waitConversion(bool celcius=true) {
    if (status == MaxStatus::converting) {
        tempCache = errorTemp;
        // max conversion time can't ever be > 1 second
        uint32_t endTime = millis() + 1000;
        while (!conversionDone())
            if (millis() >= endTime) {
                status = MaxStatus::busError;
                break;
            }
    }
}

int16_t MaxDS18x20::getTempCx10(void) {
    if (tempCache != errorTemp)
        return tempCache;
    if (conversionDone() && status == MaxStatus::complete)
        if (oneWire.reset()) {
            oneWire.write(Ds18x20Command::readRom);
            oneWire.read_bytes(maxRom.maxRomU.bytes, sizeof(maxRom.maxRomU.bytes));
            if (maxRom.crcIsValid())
                if (oneWire.reset()) {
                    oneWire.write(Ds18x20Command::skipRom);
                    oneWire.write(Ds18x20Command::readScratchpad);
                    oneWire.read_bytes(maxMem, sizeof(maxMem));
                    if (maxMem.crcIsValid) {
                        tempCache = maxMem.getTemp() * 10 / 16;
                        return tempCache;
                } else
                    status = MaxStatus::busError;
            else
                status = MaxStatus::romBadCrc;
        } else
            status = MaxStatus::busError;
    return errorTemp;
}

int16_t MaxDS18x20::getTempFx10(void) {
    int16_t x10degrees = getTempCx10();
    if (x10degrees != errorTemp)
        return x10degrees * 18 + 320;
    else
        return errorTemp;
}
