#include <iostream>
#include <require.h>
#include <MovingAvg.h>
using namespace std;

int main(int argc, char** argv) {
    SimpleMovingAvg sma(2);
    require(sma.value() == 0);
    cout << "10 == " << sma.append(10) << endl;
    require(sma.value() == 10);
    cout << "10 == " << sma.append(10) << endl;
    require(sma.value() == 10);
    cout << "15 == " << sma.append(20) << endl;
    require(sma.value() == 15);
    cout << "20 == " << sma.append(20) << endl;
    require(sma.value() == 20);
    cout << "10 == " << sma.append(0) << endl;
    require(sma.value() == 10);
    cout << "0 == " << sma.append(0) << endl;
    require(sma.value() == 0);
    return EXIT_SUCCESS;
}
