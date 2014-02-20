#include <iostream>
#include <sstream>
#include <TimedEvent.h>
#include <require.h>

const int REQUIRED_TRIGGERS = 1;
int eventTriggered = 0;

void TestEvent(const TimedEvent* timed_event) {
    eventTriggered +=1;
}

int main(int argc, char *argv[]) {
    Millis current_time=0;
    Millis interval_time=100;
    TimedEvent timed_event(current_time, interval_time, TestEvent);
    current_time = 100;
    timed_event.update(); // immediate trigger
    current_time = 0;

    Millis expected = MAX_MILLIS - 100;
    Millis received = timed_event.elapsed();

    std::stringstream msgbuf;
    msgbuf << "Elapsed wrap calculation incorrect, expecting "
        << expected << ", received " << received << ".\n";
    std::string msg = msgbuf.str();

    require(received == expected, msg.c_str());

    return !(eventTriggered == REQUIRED_TRIGGERS);
}
