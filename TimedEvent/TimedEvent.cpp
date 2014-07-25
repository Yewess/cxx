
#include <stdint.h>
#include "TimedEvent.h"

TimedEvent::TimedEvent(const Millis& current_time,
                       const Millis& interval_time,
                       EventFunc event_func)
                       :
                       currentTime(current_time),
                       intervalTime(interval_time),
                       eventFunc(event_func),
                       previousTime(currentTime),
                       elapsedTime(0) {}

bool TimedEvent::reInitIfNot(EventFunc event_func) {
    if (eventFunc != event_func) {
        eventFunc = event_func;
        previousTime = 0;
        elapsedTime = 0;
        return true;
    } else {
        return false;
    }
}

const Millis& TimedEvent::elapsed(void) {
    static Millis elapsed_result;
    elapsed_result = currentTime - previousTime;
    if (currentTime <= previousTime) {
        elapsed_result = currentTime + (MAX_MILLIS - previousTime);
    }
    return elapsed_result;
}

bool TimedEvent::update(void) {
    elapsedTime = elapsed();
    if (elapsedTime >= intervalTime) {
        previousTime = currentTime;
        (*eventFunc)(this);
        return true;
    } else {
        return false;
    }
}

void TimedEvent::reset(void) {
    elapsedTime = 0;
    previousTime = currentTime;
}
