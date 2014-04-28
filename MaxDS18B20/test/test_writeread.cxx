#include <iostream>
#include <require.h>
#include <FakeOneWire.h>
#include <MaxDS18B20.h>

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
    max.setHighAlarm(99);
    max.setLowAlarm(-98);
    max.setResolution(10);

    cout << "Writing..." << endl;
    require(max.writeMem());
    const uint8_t expected[ow.FAKE_ONEWIRE_MEMDATA_LEN] = {
                                            max.DS18B20_WRITE_MEM,
                                            (uint8_t)99,
                                            (uint8_t)-98,
                                            (uint8_t)( (10 - 9) << 5),
                                            };
    for (uint8_t c=0; c < 4; c++) {
        cout << (int)ow.memdata[c] << " ?= " << (int)expected[c] << endl;
        require(ow.memdata[c] == expected[c]);
    }

    // clear memdata
    for (uint8_t c=0; c < ow.FAKE_ONEWIRE_MEMDATA_LEN; c++)
        ow.memdata[c] = 0;
    ow.memdata[0] = max.DS18B20_READ_MEM;
    ow.memdata[1] = 0xAA;  // temp msb
    ow.memdata[2] = 0x55;  // temp lsb
    ow.memdata[3] = 0xAA;  // high_alarm
    ow.memdata[4] = 0x55;  // low_alarm
    ow.memdata[5] = 0xAA;  // configuration
    ow.memdata[9] = ow.crc8(&ow.memdata[1], max.DS18B20_MEM_BYTES-1);
    cout << "Reading..." << endl;
    require(max.readMem());
    cout << "Check high alarm" << endl;
    require(max.getHighAlarm() == (int8_t)0xAA);
    cout << "Check low alarm" << endl;
    require(max.getLowAlarm() == (int8_t)0x55);
    cout << "Check resolution" << endl;
    require(max.getResolution() == (uint8_t)0xa); // from 0xAA
    return EXIT_SUCCESS;
}
