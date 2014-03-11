#include <iostream>
#include <require.h>
#include <MovingAvg.h>
using namespace std;

int main(int argc, char** argv) {
    SimpleMovingAvg sma(3);
    require(sma.value() == 0);

    cout << "(10)/1 == " << sma.append(10) << endl;
    require(sma.value() == 10);

    cout << "(10+10)/2 == " << sma.append(10) << endl;
    require(sma.value() == 10);

    cout << "(10+10+20)/3 == " << sma.append(20) << endl;
    require(sma.value() == 13);

    cout << "(10+20+30)/3 == " << sma.append(30) << endl;
    require(sma.value() == 20);

    return EXIT_SUCCESS;
}
