#include <cstddef>
#include <cstdint>
#include <iostream>
#include "require.h"
#include <StateMachine.h>

static int dummy_call_count = 0;

void dummy(StateMachine* state_machine) {
    dummy_call_count++;
}

int main(int argc, char* argv[]) {
    using namespace std;
    enum { zero, one, two, _max };
    StateMachine state_machine(_max, _max + 1);
    cout << "Starting ID " << (int) state_machine.getCurrentId() << endl;
    state_machine.define(one, dummy);
    state_machine.define(one, dummy);
    state_machine.define(one, dummy);
    cout << "Three calls to state one defined" << endl;
    state_machine.next();
    cout << "Next ID " << (int) state_machine.getCurrentId() << endl;
    state_machine.service();
    cout << "Actual calls: " << dummy_call_count << endl;
    require(dummy_call_count == 3);
    state_machine.next();
    cout << "Next ID " << (int) state_machine.getCurrentId() << endl;
    state_machine.service();
    cout << "Actual calls: " << dummy_call_count << endl;
    require(dummy_call_count == 6);
    return EXIT_SUCCESS;
}
