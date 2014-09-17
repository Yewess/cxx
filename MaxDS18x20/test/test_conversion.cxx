#include <iostream>
#include <require.h>
#include <FakeOneWire.h>
#include <MaxDS18x20.h>

using namespace std;

void setup_temp(OneWire& ow, MaxDS18B20& max,
               uint16_t temp, uint8_t resolution) {
    cout << "Converting temperature" << endl;
    require(max.startConversion());
    require(ow.memdata[0] == max.DS18B20_CONVERT_T);
    cout << "Getting temperature" << endl;
    ow.memdata[0] = 0; // fake still converting
    require(max.conversionDone() == false);
    ow.memdata[0] = 1; // fake done converting
    require(max.conversionDone() == true);
    cout << "Setting up fake memory" << endl;
    // clear memdata
    for (uint8_t c=0; c < ow.FAKE_ONEWIRE_MEMDATA_LEN; c++)
        ow.memdata[c] = 0;
    // 125C
    ow.memdata[1] = temp & ~0xFF00; // temp lsb
    ow.memdata[2] = temp >> 8; // temp msb
    ow.memdata[3] = 0x00;  // high alarm
    ow.memdata[4] = 0x00;  // low_alarm
    cout << "Setting resolution to " << (uint16_t) resolution << " bits"
         << endl;
    ow.memdata[5] = (resolution - 9) << 5;
    ow.memdata[9] = ow.crc8(&ow.memdata[1], max.DS18B20_MEM_BYTES-1);
    cout << "Reading Memory..." << endl;
    require(max.readMem());
}

typedef struct temp_bits_t {
    uint16_t temp;
    uint8_t bits;
    int16_t tempC;
    int16_t tempF;
} temp_bits_t;

const temp_bits_t temp_bits[] = {
    {0x0008, 12, 50, 3290},
    {0x0191, 12, 2506, 7710},
    {0x0550, 12, 8500, 18500},
    {0x07D0, 12, 12500, 25700},
    {0x03e8, 11, 12500, 25700},
    {0x01f7, 10, 12500, 25700},
    {0x00ff, 9, 12400, 25520},
    {0, 12, 0, 3200},
    {0, 11, 0, 3200},
    {0, 10, 0, 3200},
    {0, 9, 0, 3200},
    {0xFFF8, 12, -50, 3110},
    {0xFF5E, 12, -1012, 1379},
    {0xFE6F, 12, -2506, -1310},
    {0xFC90, 12, -5500, -6700},
};
const uint8_t tblen = sizeof(temp_bits) / sizeof(temp_bits_t);

int main(int argc, char** argv) {
    using namespace std;

    OneWire ow;

    MaxDS18B20::MaxRom rom;
    rom.rom.family = MaxDS18B20::DS18B20_FAMILY_CODE;
    for (uint8_t c=0; c < MaxDS18B20::DS18B20_SERIAL_BYTES; c++)
        rom.rom.serial[c] = c;
    rom.rom.crc8 = ow.crc8(rom.bytes, sizeof(rom) - 1);
    require(rom.crcIsValid());
    require(rom.isDS18B20());
    cout << "Fake rom is setup okay" << endl;
    MaxDS18B20 max(ow, rom);

    for (uint8_t i=0; i<tblen; i++) {
        setup_temp(ow, max, temp_bits[i].temp, temp_bits[i].bits);
        cout << "Received TempC " << (int16_t) max.getTempC() << endl;
        require(max.getTempC() == temp_bits[i].tempC);
        cout << "Received TempF " << (int16_t) max.getTempF() << endl;
        require(max.getTempF() == temp_bits[i].tempF);
    }
    return EXIT_SUCCESS;
}
