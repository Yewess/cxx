#include <cstddef>
#include <cstdint>
#include <iostream>
#include "require.h"
#include <StateMachine.h>

using namespace std;

enum { ZERO, ONE, TWO, THREE, _MAX };

static int dummy_one_call_count = 0;
static int dummy_two_call_count = 0;
static int dummy_three_call_count = 0;

void dummy_one(StateMachine* state_machine) {
    cout << "Dummy One called, ID " << (int) state_machine->getCurrentId() << endl;
    dummy_one_call_count++;
    state_machine->next();
    cout << "Dummy One next ID " << (int) state_machine->getCurrentId() << endl;
}

void dummy_two(StateMachine* state_machine) {
    cout << "Dummy Two called, ID " << (int) state_machine->getCurrentId() << endl;
    dummy_two_call_count++;
    state_machine->undefine(TWO);
    cout << "Dummy Two undefined Two, total " << (int) state_machine->defined() << endl;
    state_machine->next();
    cout << "Dummy Two next ID " << (int) state_machine->getCurrentId() << endl;
}

void dummy_three(StateMachine* state_machine) {
    cout << "Dummy Three called, ID " << (int) state_machine->getCurrentId() << endl;
    dummy_three_call_count++;
    state_machine->next();
    cout << "Dummy Three next ID " << (int) state_machine->getCurrentId() << endl;
}

int main(int argc, char* argv[]) {
    StateMachine state_machine(_MAX, ZERO);
    cout << "Starting ID " << (int) state_machine.getCurrentId() << endl;
    state_machine.define(ONE, dummy_one);
    state_machine.define(TWO, dummy_two);
    state_machine.define(THREE, dummy_three);
    cout << (int) state_machine.defined() << " calls defined" << endl;
    cout << "Starting ID " << (int) state_machine.getCurrentId() << endl;

    state_machine.next();
    cout << "Next ID " << (int) state_machine.getCurrentId() << endl;

    state_machine.service();
    cout << "Second ID " << (int) state_machine.getCurrentId() << endl;

    state_machine.service();
    cout << "Third ID " << (int) state_machine.getCurrentId() << endl;

    state_machine.service();
    cout << "Forth ID " << (int) state_machine.getCurrentId() << endl;

    state_machine.service();
    cout << "Fith ID " << (int) state_machine.getCurrentId() << endl;

    state_machine.service();
    cout << "Sixth ID " << (int) state_machine.getCurrentId() << endl;

    state_machine.service();
    cout << "Total calls: One- " << dummy_one_call_count
         << " Two- " << dummy_two_call_count << " Three- "
         << dummy_three_call_count << endl;
    require(dummy_one_call_count == 3);
    require(dummy_two_call_count == 1);
    require(dummy_three_call_count == 2);
    require(state_machine.getCurrentId() == 3);
    return EXIT_SUCCESS;
}
