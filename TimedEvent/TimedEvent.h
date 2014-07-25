
#ifndef TIMEDEVENT_H
#define TIMEDEVENT_H

#include <stdint.h>

typedef unsigned long Millis;

const unsigned long MAX_MILLIS = (unsigned long)-1;

class TimedEvent;
typedef class TimedEvent TimedEvent;
typedef void (*EventFunc)(TimedEvent* timed_event);

class TimedEvent {
    private:
        /* Data */
        const Millis& currentTime; /* Maintained Externally */
        const Millis& intervalTime; /* Maintained Externally */
        EventFunc eventFunc; /* Maintained Externally */
        Millis previousTime;
        Millis elapsedTime;
    public:
        /* Member Functions */
        // constructor
        TimedEvent(const Millis& current_time,
                   const Millis& interval_time,
                   EventFunc event_func);
        // return true if eventFunc updated to event_func
        bool reInitIfNot(EventFunc event_func);
        const Millis& elapsed(void);
        // returns elapsed time (assumes previousTime != 0 != elapsedTime)
        bool update(void);
        // returns true if eventFunc called
        void reset(void);
        // Begin timing period from start
};
#endif // TIMEDEVENT_H
