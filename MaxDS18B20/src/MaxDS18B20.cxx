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

#include "MaxDS18B20.h"

/*                                               *\
 *                  Public                       *
\*                                               */

#ifdef __AVR__
void MaxDS18B20::MaxRom::serial_print(void) const {
    Serial.print(F("Family: 0x"));
    Serial.print(rom.family, HEX);
    Serial.print(F(" Serial: 0x"));
    for (uint8_t idx=0; idx < DS18B20_SERIAL_BYTES; idx++)
        Serial.print(serial[idx], HEX)
    Serial.print(F(" CRC8: 0x"));
    Serial.println(crc8, HEX);
}
#endif // __AVR__

// Return false if no more devices, otherwise fill
bool MaxDS18B20::nextDS18B20(OneWire& one_wire,
                             MaxRom& max_Rom) {
  one_wire.target_search(DS18B20_FAMILY_CODE);
  if (one_wire.search(max_Rom.bytes) && max_Rom.crcIsValid())
      return one_wire.reset(); // device still there
  // Wires shorted, bad crc, read error, etc.
  return false;
}

bool MaxDS18B20::getMaxROM(OneWire& one_wire,
                           MaxRom& max_Rom,
                           uint8_t index) {
    uint8_t c = 0;
    // nuke max_Rom
    for (; c < DS18B20_ROM_BYTES; c++)
        max_Rom.bytes[c] = 0;
    c = 0;
    one_wire.reset_search();
    // loop until no more found or Wires shorted, bad crc, read error, etc.
    while (MaxDS18B20::nextDS18B20(one_wire, max_Rom)) {
        if (c == index)
            return true;
        c++;
        if (c > 100)
            return false; // safety net
    }
    return false;
}

bool MaxDS18B20::readMem(void) {
    if (locked)
        return false;
    if (maxRom.crcIsValid() && maxRom.isDS18B20() && oneWire.reset()) {
        oneWire.select(maxRom.bytes);
        oneWire.write(DS18B20_READ_MEM);  // Read Scratchpad
        oneWire.read_bytes(maxMem.bytes, DS18B20_MEM_BYTES);
        return maxMem.crcIsValid();
    } else
        return false;
}

bool MaxDS18B20::writeMem(void) const {
    if (locked)
        return false;
    if (maxRom.crcIsValid() && maxRom.isDS18B20() && oneWire.reset()) {
        uint8_t write_buf[4] = {DS18B20_WRITE_MEM,
                                (uint8_t)maxMem.mem.high_alarm,
                                (uint8_t)maxMem.mem.low_alarm,
                                maxMem.mem.configuration};
        oneWire.select(maxRom.bytes);
        oneWire.write_bytes(write_buf, 4);
        return true;
    } else
        return false;
}

bool MaxDS18B20::conversionDone(void) {
    if (locked)
        if (oneWire.read_bit()) {
            locked = false;
            return true;
        }
    return false;
}

bool MaxDS18B20::startConversion(bool all) {
    if (locked)
        return false;
    if (maxRom.crcIsValid() && maxRom.isDS18B20() && oneWire.reset()) {
        if (all)
            oneWire.write(DS18B20_SKIP_ROM);
        else
            oneWire.select(maxRom.bytes);
        oneWire.write(DS18B20_CONVERT_T);
        if (oneWire.read_bit()) {// Can't be used with parasite power :(
            locked = true;
            return true;
        } else
            return false;
    } else
        return false;
}

bool MaxDS18B20::saveMem(bool all) const {
    if (locked)
        return false;
    if (maxRom.crcIsValid() && maxRom.isDS18B20() && oneWire.reset()) {
        if (all)
            oneWire.write(DS18B20_SKIP_ROM);
        else
            oneWire.select(maxRom.bytes);
        oneWire.write(DS18B20_SAVE_MEM);
        return true;
    } else
        return false;
}

bool MaxDS18B20::loadMem(bool all) {
    if (locked)
        return false;
    if (maxRom.crcIsValid() && maxRom.isDS18B20() && oneWire.reset()) {
        if (all)
            oneWire.write(DS18B20_SKIP_ROM);
        else
            oneWire.select(maxRom.bytes);
        oneWire.write(DS18B20_LOAD_MEM);
        if (oneWire.read_bit()) { // Can't be used with parasite power :(
            locked = true;
            return true;
        } else
            return false;
    } else
        return false;
}

int16_t MaxDS18B20::getTempC(void) {
    return ( (int32_t)maxMem.mem.temperature * 100 ) / 1600;
}

int16_t MaxDS18B20::getTempF(void) {
    return ( (int32_t)getTempC() * 180 ) + 3200;
}

/*                                               *\
 *                  Private
\*                                               */

#ifdef __AVR__
void MaxDS18B20::MaxMem::serial_print(void) const {
    Serial.print(F("Temperature: "));
    Serial.print(mem.temperature);
    Serial.print(F(" High Alarm: "));
    Serial.print(mem.high_alarm);
    Serial.print(F(" Low Alarm: "));
    Serial.print(mem.low_alarm);
    Serial.print(F(" Configuration: 0x"));
    Serial.print((mem.configuration, HEX);
    Serial.print(F(" CRC8: 0x"));
    Serial.println(mem.crc8, HEX);
}
#endif // __AVR__
