#include <iostream>
#include <require.h>
#include <FakeOneWire.h>
#include <MaxDS18B20.h>

int main(int argc, char** argv) {
    using namespace std;

    OneWire ow;
    MaxDS18B20::MaxRom rom;
    MaxDS18B20 max(ow, rom);
    for (uint8_t c=0; c < ow.FAKE_ONEWIRE_MEMDATA_LEN; c++)
        ow.memdata[c] = 0;

    cout << "Rom size: " << sizeof(rom) << endl;
    cout << "Max size: " << sizeof(max) << endl;

    for (uint8_t c=0; c < ow.FAKE_ONEWIRE_MEMDATA_LEN; c++)
        require(ow.memdata[c] == 0);
    return EXIT_SUCCESS;
}
