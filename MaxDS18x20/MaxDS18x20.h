
#ifndef MAXDS18X20_H
#define MAXDS18X20_H

#ifdef __AVR__
    #include <stdint.h>
    #include <avr/pgmspace.h>
    #include <Arduino.h>
    #include <OneWire.h>
#else
    #include <cstring>
    #include <cstddef>
    #include <cstdint>
    #include <FakeOneWire.h>
#endif //__AVR__

static const maxRommaxRomSerialBytes = 6;

union MaxRom;
typedef union MaxRomU {
    private:
        /*
        ** DATA MEMBERS
        */
        #ifdef __AVR__
            struct {
                uint8_t family;
                uint8_t serial[maxRomSerialBytes];
                uint8_t crc8;
            } rom;
        #else
            struct alignas(uint8_t) {
                uint8_t family :8;
                uint8_t serial[maxRomSerialBytes];
                uint8_t crc8 :8;
            } rom;
        #endif // __AVR__
    public:
        /*
        ** DATA MEMBERS
        */
        uint8_t bytes[sizeof(rom)];
        /*
        ** MEMBER FUNCTIONS
        */
        MaxRomU(void) {
            memset(bytes, 0, sizeof(rom));
        };
        bool crcIsValid(void) const {
            return  OneWire::crc8(bytes, sizeof(rom) - 1 ) == rom.crc8;
        };
        bool isDS18x20(void) const {
            return ((rom.family == 0x10) ||
                    (rom.family == 0x28) ||
                    (rom.family == 0x22));
        };
        bool isTypeS(void) const {
            return rom.family == 0x10;
        };
        // do not waste space by storing crc8
        void dump(uint8 dest[sizeof(rom) - 1]) const {
            // Family code + serial number
            memcpy(dest, bytes, sizeof(rom) - 1);
        };
        void load(uint8 src[sizeof(rom)] - 1) {
            memcpy(bytes, src, sizeof(rom) - 1);
            // Re-calculate crc8
            rom.crc8 = OneWire::crc8(bytes, sizeof(rom) - 1);
        };
} MaxRomU;

typedef struct MaxRom {
    /*
    ** DATA MEMBERS
    */
    MaxRomU maxRomU;
    int8_t tempOffset;
    /*
    ** MEMBER FUNCTIONS
    */
    MaxRom(void) maxRomU(), tempOffset(0) {};
    void dump(uint8 dest[sizeof(maxRomU)]) const {
        maxRomU.dump(dest);
        dest[sizeof(maxRomU) - 1] = tempOffset;
    };
    void load(uint8 src[sizeof(rom)]) {
        tempOffset = dest[sizeof(maxRomU) - 1];
        maxRomU.load(src);
    };
}

static const int16_t errorTemp = -32766;

union MaxMem;
typedef union MaxMem {
    private:
        /*
        ** DATA MEMBERS
        */
        #ifdef __AVR__
            struct {
                int16_t temperature;
                int8_t  highAlarm;
                int8_t  lowAlarm;
                uint8_t configuration;
                uint8_t reserved1;
                uint8_t reserved2;
                uint8_t reserved3;
                uint8_t crc8;
            } mem;
        #else
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
        #endif // __AVR__
        /*
        ** MEMBER FUNCTIONS
        */
        void resetCrc8(void) {
            mem.crc8 = OneWire::crc8(bytes, sizeof(mem) - 1);
        };
    public:
        /*
        ** DATA MEMBERS
        */
        uint8_t bytes[sizeof(mem)];
        /*
        ** MEMBER FUNCTIONS
        */
        MaxMem(void) {
            memset(bytes, 0, sizeof(mem);
        };
        bool crcIsValid(void) const {
            return OneWire::crc8(bytes, sizeof(mem) - 1) == mem.crc8;
        };
        uint8_t getResolution(void) const {
            // seventh and first 5 bits are reserved, 9 is offset
            return ((mem.configuration & 0x60) >> 5) + 9;
        };
        uint8_t setResolution(uint8_t n_bits) {
            if ((n_bits >= 9) && (n_bits <= 12)) {
                mem.configuration = n_bits = n_bits - 9 << 5;
                resetCrc8();
            }
            return getResolution();
        };
        int16_t getTemp(const MaxRom& max_rom) const
        int16_t getHighAlarm(void) const { return mem.highAlarm * 10; };
        int16_t setHighAlarm(int16_t TempCx10) {
            mem.highAlarm = TempCx10 / 10;
            resetCrc8();
            return mem.highAlarm * 10;
        };
        int16_t getLowAlarm(void) const { return mem.LowAlarm * 10; };
        int16_t setLowAlarm(int16_t TempCx10) {
            mem.lowAlarm = TempCx10 / 10;
            resetCrc8();
            return mem.lowAlarm * 10;
        };
} MaxMem;

struct Ds18x20Command {
    static const uint8_t readRom = 0x33;
    static const uint8_t skipRom = 0xCC;
    static const uint8_t alarMSearch = 0xEC;
    static const uint8_t convertT = 0x44;
    static const uint8_t writeScratchpad = 0x4E;
    static const uint8_t readScratchpad = 0xBE;
    static const uint8_t copyScratchpad = 0x48;
    static const uint8_t recallEE = 0xB8;
};


struct MaxStatus {
    static const uint8_t converting = 1;
    static const uint8_t notFound = 3;
    static const uint8_t romBadCrc = 4;
    static const uint8_t ramBadCrc = 5;
    static const uint8_t busError = 6;
    static const uint8_t notDs18x20 = 7;
    static const uint8_t complete = 8;
    static const uint8_t noStatus = 9;
};


struct MaxDS18x20 {
    public:
        /*
        ** MEMBER FUNCTIONS
        */
        MaxDS18B20(OneWire& one_wire)
                 : oneWire(one_wire), maxRom(),
                   maxMem(), status(MaxStatus::noStatus) {};
        uint8_t status(void) { return status };
        uint8_t startConversion(void);
        bool conversionDone(void);
        void waitConversion(void);
        int16_t getTempCx10(void);
        int16_t getTempFx10(void);
        int16_t fToC(int16_t temp_f_x10) {
            return temp_f_x10 * 18 / 10 + 320;
        }
    private:
        /*
        ** DATA MEMBERS
        */
        OneWire& oneWire;
        MaxRom maxRom;
        MaxMem maxMem;
        uint8_t status;
        int16_t tempCache;
};

#endif // MAXDS18X20_H
