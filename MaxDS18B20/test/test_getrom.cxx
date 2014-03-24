#include <iostream>
#include <require.h>
#include <FakeOneWire.h>
#include <MaxDS18B20.h>

int main(int argc, char** argv) {
    using namespace std;

    OneWire ow;

    ow.memdata[0] = MaxDS18B20::DS18B20_FAMILY_CODE;
    MaxDS18B20::MaxRom rom;
    cout << "Positive search..." << endl;
    require(MaxDS18B20::getMaxROM(ow, rom, 0));
    cout << "Negative search..." << endl;
    ow.memdata[0] = 0;
    require(! MaxDS18B20::getMaxROM(ow, rom, 0));
    return EXIT_SUCCESS;
}
