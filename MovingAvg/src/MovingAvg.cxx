#include <cstddef>
#include <cstdint>
#include "MovingAvg.h"

int16_t SimpleMovingAvg::append(int16_t data_point) {
    if (nPoints > 0) {
        float total = 0;
        if (nPoints < maxPoints) {
            // keep appending
            total = dataPoints[nPoints] = data_point;
            for (uint16_t index=0; index < nPoints; index++)
                total += dataPoints[index];
            nPoints++;
        } else {
            nPoints = maxPoints; // make sure
            // make room by shifting down, add up total also
            for (uint16_t index=1; index < maxPoints; index++) {
                dataPoints[index - 1] = dataPoints[index];
                total += dataPoints[index];
            }
            dataPoints[maxPoints-1] = data_point;
            total += data_point;
        }
        // guarantee float math
        total /= (float)nPoints;
        // NOW truncate
        currentSMA = total;
    } else {
        if (maxPoints > 0) {
            currentSMA = dataPoints[0] = data_point;
            nPoints++;
        }
    }
    return currentSMA;
}
