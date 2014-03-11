
#ifndef MOVINGAVG_H
#define MOVINGAVG_H

#include <cstddef>
#include <cstdint>

class SimpleMovingAvg {
    private:
        /* Data */
        const uint16_t maxPoints;
        uint16_t nPoints;
        int16_t* dataPoints;
        int16_t currentSMA;
    public:
        /* Member Functions */
        // constructor
        SimpleMovingAvg(uint16_t max_points);
        // destructor
        ~SimpleMovingAvg();
        // append new data point, return current value
        int16_t append(int16_t data_point);
        // retrieve current mean value
        int16_t value(void) const;
};

inline SimpleMovingAvg::SimpleMovingAvg(uint16_t max_points)
                                      :
                                      maxPoints(max_points),
                                      nPoints(0),
                                      currentSMA(0) {
    dataPoints = new int16_t[maxPoints];
}

inline SimpleMovingAvg::~SimpleMovingAvg(void) {
    if (dataPoints != NULL) {
        delete dataPoints;
        dataPoints = NULL;
    }
    nPoints = 0;
    currentSMA = 0;
}

inline int16_t SimpleMovingAvg::value(void) const {
    return currentSMA;
}

#endif // MOVINGAVG_H
