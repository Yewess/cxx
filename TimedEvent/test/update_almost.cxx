#include <TimedEvent.h>
#include <require.h>

const int REQUIRED_TRIGGERS = 0;
int eventTriggered = 0;

void TestEvent(const TimedEvent* timed_event) {
    eventTriggered +=1;
}

int main(int argc, char *argv[]) {
    Millis current_time=0;
    Millis interval_time=100;
    TimedEvent timed_event(current_time, interval_time, TestEvent);
    current_time = 99;
    timed_event.update();
    require(eventTriggered == 0, "Event triggered too early");
    return !(eventTriggered == REQUIRED_TRIGGERS);
}
