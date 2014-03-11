#include <iostream>
#include <require.h>
#include <MovingAvg.h>

using namespace std;

int main(int argc, char** argv) {
    SimpleMovingAvg sma(1);
    require(sma.value() == 0);

    cout << "12345 == " << sma.append(12345) << endl;
    require(sma.value() == 12345);

    cout << "4321 == " << sma.append(4321) << endl;
    require(sma.value() == 4321);

    return EXIT_SUCCESS;
}
