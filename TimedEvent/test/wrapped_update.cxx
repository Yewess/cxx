#include <iostream>
#include <sstream>
#include <TimedEvent.h>
#include <require.h>

const int REQUIRED_TRIGGERS = 1;
int eventTriggered = 0;

void TestEvent(TimedEvent* timed_event) {
    eventTriggered +=1;
}

int main(int argc, char *argv[]) {
    Millis current_time = 0;
    Millis interval_time = MAX_MILLIS - 1000;
    TimedEvent timed_event(current_time, interval_time, TestEvent);
    current_time = 1;
    timed_event.update(); // previousTime == 0
    current_time = 2;
    timed_event.update(); // previousTime == 1
    current_time = 0;
    std::cout << "eventTriggered pre == " << eventTriggered << std::endl;
    std::cout << "timed_event.elapsed pre-update == " << timed_event.elapsed()
        << std::endl;
    timed_event.update(); // trigger
    std::cout << "eventTriggered post == " << eventTriggered << std::endl;
    std::cout << "timed_event.elapsed post-update == " << timed_event.elapsed()
        << std::endl;
    return !(eventTriggered == REQUIRED_TRIGGERS);
}
