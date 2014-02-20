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
    current_time=1;
    timed_event.update();
    require(timed_event.elapsed() == 1, "Initial elapsed not 0");
    current_time = 50;
    require(timed_event.elapsed() == 50, "Fixed elapsed not 50");
    return !(eventTriggered == REQUIRED_TRIGGERS);
}
