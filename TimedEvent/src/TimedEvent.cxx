
#include <stdint.h>
#include "TimedEvent.h"

TimedEvent::TimedEvent(const Millis& current_time,
                       const Millis& interval_time,
                       EventFunc event_func)
                       :
                       currentTime(current_time),
                       intervalTime(interval_time),
                       eventFunc(event_func),
                       previousTime(current_time),
                       elapsedTime(0) {}

bool TimedEvent::reInitIfNot(EventFunc event_func) {
    if (eventFunc != event_func) {
        eventFunc = event_func;
        previousTime = currentTime;
        elapsedTime = 0;
        return true;
    } else {
        return false;
    }
}

const Millis& TimedEvent::elapsed(void) {
    static Millis elapsed_result;
    if (currentTime <= previousTime) { // currentTime has wrapped
        elapsed_result = ((unsigned long)-1) - previousTime;
        elapsed_result += currentTime;
    } else { // no wrap
        elapsed_result = currentTime - previousTime;
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
