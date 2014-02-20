#include <iostream>
#include <TimedEvent.h>
#include <require.h>

const int REQUIRED_TRIGGERS = 1;
int eventTriggered = 0;

void TestEvent(const TimedEvent* timed_event) {
    eventTriggered += 1;
}

int main(int argc, char *argv[]) {
    Millis current_time=0;
    Millis interval_time=100;
    TimedEvent timed_event(current_time, interval_time, TestEvent);
    current_time = 50;
    timed_event.update(); // no trigger
    std::cout << "eventTriggered == " << eventTriggered << std::endl;
    current_time = 100;
    timed_event.update(); // trigger
    std::cout << "eventTriggered == " << eventTriggered << std::endl;
    current_time = 150;
    timed_event.update(); // no trigger
    std::cout << "eventTriggered == " << eventTriggered << std::endl;
    return !(eventTriggered == REQUIRED_TRIGGERS);
}
