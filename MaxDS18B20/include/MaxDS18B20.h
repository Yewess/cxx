
#ifndef MAXDS18B20_H
#define MAXDS18B20_H

#ifdef __AVR__
    #include <stdint.h>
    #include <Arduino.h>
    #include <OneWire.h>
#else
    #include <cstring>
    #include <cstddef>
    #include <cstdint>
    #include <FakeOneWire.h>
#endif //__AVR__

struct MaxDS18B20 {
    public:

        // Data Members
        static const uint8_t DS18B20_FAMILY_CODE = 0x28;
        static const uint8_t DS18B20_SERIAL_BYTES = 6;
        static const uint8_t DS18B20_ROM_BYTES = 8;
        static const uint8_t DS18B20_MEM_BYTES = 9;
        static const uint8_t DS18B20_READ_MEM = 0xBE;
        static const uint8_t DS18B20_CONVERT_T = 0x44;
        static const uint8_t DS18B20_SKIP_ROM = 0xCC;
        static const uint8_t DS18B20_WRITE_MEM = 0x4E;
        static const uint8_t DS18B20_SAVE_MEM = 0x48;
        static const uint8_t DS18B20_LOAD_MEM = 0xB8;
        // Types
        union MaxRom {

            // Data Members
            struct alignas(uint8_t) {
                uint8_t family :8;
                uint8_t serial[DS18B20_SERIAL_BYTES];
                uint8_t crc8 :8;
            } rom;
            uint8_t bytes[DS18B20_ROM_BYTES];

            // Member Functions
            bool crcIsValid(void) const;
            bool isDS18B20(void) const;
#ifdef __AVR__
            String toStr(void) const;
#endif // __AVR__
        };

        // Member Functions
        MaxDS18B20(OneWire& one_wire, const MaxRom& max_Rom);  // Constructor
        // Populate max_Rom with one_wire device @ index, return true on success
        static bool getMaxROM(OneWire& one_wire,
                              MaxRom& max_Rom,
                              uint8_t index);
        bool readMem(void);  // true if Rom & mem valid
        bool writeMem(void) const;  // true on success
        // Begin locked temp. value, for one or all devices
        bool startConversion(bool all=false);  // true on success
        // store device mem in device eepRom, for one or all devices
        bool saveMem(bool all=false) const;  // true on success
        // retrieve device eepRom to device mem, for one or all devices
        bool loadMem(bool all=false);  // true on success
        bool conversionDone(void);  // true when complete
        // Retrieve current value of alarms (in C)
        int8_t getHighAlarm(void) const;
        int8_t getLowAlarm(void) const;
        // retrieve temperature resolution number of bits (12, 11, 10, 9)
        uint8_t getResolution(void) const;
        // set new value for alarms
        void setHighAlarm(int8_t value);
        void setLowAlarm(int8_t value);
        // set temperature resolution number of bits (12, 11, 10, 9)
        void setResolution(uint8_t n_bits);
        // return temperature in Celsius x 100
        int16_t getTempC(void);
        // return temperature in Fahrenheit x 100
        int16_t getTempF(void);
    private:

        // Data Members
        OneWire& oneWire;
        const MaxRom& maxRom;
        bool locked;  // call to startConversion is uninterrupted
        union alignas(uint8_t) MaxMem {

            // Data Members
            struct alignas(uint8_t) {
                int16_t temperature :16;
                int8_t  high_alarm :8;
                int8_t  low_alarm :8;
                uint8_t configuration :8;
                uint8_t reserved_1 :8;
                uint8_t reserved_2 :8;
                uint8_t reserved_3 :8;
                uint8_t crc8 :8;
            } mem;
            uint8_t bytes[DS18B20_MEM_BYTES];

            // Member Functions
            bool crcIsValid(void) const;
#ifdef __AVR__
            String toStr(void) const;
#endif // __AVR__
        } maxMem;
        // Member Functions
        static bool nextDS18B20(OneWire& one_wire,
                                MaxRom& max_Rom);
};


/*                                               *\
 *                   Public
\*                                               */

inline MaxDS18B20::MaxDS18B20(OneWire& one_wire,
                              const MaxRom& max_Rom)
                              :
                              oneWire(one_wire),
                              maxRom(max_Rom),
                              locked(false) {
    for (uint8_t c=0; c < DS18B20_MEM_BYTES; c++)
        maxMem.bytes[c] = 0;
}

inline bool MaxDS18B20::MaxRom::crcIsValid(void) const {
    return  OneWire::crc8(bytes, DS18B20_ROM_BYTES - 1 ) == rom.crc8;
}

inline bool MaxDS18B20::MaxRom::isDS18B20(void) const {
    return rom.family == DS18B20_FAMILY_CODE;
}

inline int8_t MaxDS18B20::getHighAlarm(void) const {
    return maxMem.mem.high_alarm;
}

inline int8_t MaxDS18B20::getLowAlarm(void) const {
    return maxMem.mem.low_alarm;
}

inline uint8_t MaxDS18B20::getResolution(void) const {
    // seventh and first 5 bits are reserved, 9 is offset
    return ((maxMem.mem.configuration & 0x60) >> 5) + 9;
}

inline void MaxDS18B20::setHighAlarm(int8_t value) {
    maxMem.mem.high_alarm = value;
    maxMem.mem.crc8 = 0;
}

inline void MaxDS18B20::setLowAlarm(int8_t value) {
    maxMem.mem.low_alarm = value;
    maxMem.mem.crc8 = 0;
}

inline void MaxDS18B20::setResolution(uint8_t n_bits) {
    if ((n_bits >= 9) && (n_bits <= 12)) {
        n_bits -= 9;
        n_bits = n_bits << 5;
        maxMem.mem.configuration = n_bits;
        maxMem.mem.crc8 = 0;
    }
}

/*                                               *\
 *                   Private
\*                                               */

inline bool MaxDS18B20::MaxMem::crcIsValid(void) const {
    return OneWire::crc8(bytes, DS18B20_MEM_BYTES - 1) == mem.crc8;
}

#endif // MAXDS18B20_H
