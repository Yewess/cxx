#include <iostream>
#include <require.h>
#include <MovingAvg.h>

int main(int argc, char** argv) {
    SimpleMovingAvg sma(0);
    require(sma.value() == 0);
    sma.append(12345);
    require(sma.value() == 0);
    return EXIT_SUCCESS;
}
