
#ifndef FAKEONEWIRE_H
#define FAKEONEWIRE_H

struct OneWire {
    public:
        static const uint8_t FAKE_ONEWIRE_MEMDATA_LEN = 10;
        uint8_t memdata[FAKE_ONEWIRE_MEMDATA_LEN];
        OneWire(void) {
            reset_search();
        }
        uint8_t reset(void) { return 1; }
        void select(const uint8_t rom[8]) {}
        void write(uint8_t v) {
            memdata[0] = v;
        }
        void write_bytes(const uint8_t *buf, uint16_t count) {
            for (uint16_t c=0; c < count; c++)
                memdata[c] = buf[c];
        }
        uint8_t read(void) {
            return memdata[0];
        }
        void read_bytes(uint8_t *buf, uint16_t count) {
            for (uint16_t c=1; c < count+1; c++)
                buf[c-1] = memdata[c];
        }
        uint8_t read_bit(void) {
            return memdata[0];
        }
        void reset_search() {
            for (uint8_t c=0; c < 6; c++)
                memdata[c+1] = c+1;
            memdata[7] = crc8(memdata, FAKE_ONEWIRE_MEMDATA_LEN);
        }
        void target_search(uint8_t family_code) {
        }
        uint8_t search(uint8_t *newAddr) {
            if (memdata[0]) {  // non-zero
                newAddr[0] = memdata[0];
                for (uint8_t c=1; c < 7;c++) {
                    newAddr[c] = memdata[c] + memdata[0];
                }
                newAddr[7] = crc8(newAddr, 7);
                if (memdata[0] != 0x28)
                    newAddr[7] += 0x28;
                return newAddr[0] == 0x28;  // true if family-code right
            } else
                return 0;
        }
        static uint8_t crc8(const uint8_t *addr, uint8_t len) {
            uint8_t result = 0;
            for (uint16_t c=0; c < len; c++)
                result += addr[c];
            return result;
        }
};

#endif // FAKEONEWIRE_H
