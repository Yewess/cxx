#include <cstddef>
#include <cstdint>
#include <iostream>
#include "require.h"
#include <StateMachine.h>

static int dummy_call_count = 0;

enum { zero, one, two, three, _max };

void dummy(StateMachine* state_machine) {
    dummy_call_count++;
    state_machine->next();
}

void foo(void) {
    using namespace std;
    StateMachine state_machine(_max, one);
    state_machine.define(one, dummy);
    state_machine.define(two, dummy);
    state_machine.define(three, dummy);
    state_machine.service();
    state_machine.service();
    state_machine.service();
    state_machine.~StateMachine();
    require(dummy_call_count == 3);
    state_machine.~StateMachine();
}

int main(int argc, char* argv[]) {
    foo();
    return EXIT_SUCCESS;
}
