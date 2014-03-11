#include <iostream>
#include <require.h>
#include <MovingAvg.h>

int main(int argc, char** argv) {
    if (true) {
        SimpleMovingAvg sma(0);
        require(sma.value() == 0);
    };
    if (true) {
        SimpleMovingAvg sma(10);
        require(sma.value() == 0);
    };
    if (true) {
        SimpleMovingAvg sma(100);
        require(sma.value() == 0);
    };
    if (true) {
        SimpleMovingAvg sma(1000);
        require(sma.value() == 0);
    };
    if (true) {
        SimpleMovingAvg sma(10000);
        require(sma.value() == 0);
    };
    if (true) {
        SimpleMovingAvg sma((uint16_t)-1);
        require(sma.value() == 0);
    };
    return EXIT_SUCCESS;
}
