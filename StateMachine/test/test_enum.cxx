#include <cstddef>
#include <cstdint>
#include <iostream>
#include "require.h"
#include <StateMachine.h>

static uint8_t dummy_call_count = 0;

void dummy(StateMachine* state_machine) {
    dummy_call_count++;
}

int main(int argc, char* argv[]) {
    using namespace std;
    enum { zero, one, two, _max };
    StateMachine state_machine(_max);
    cout << "define 0" << endl;
    require(state_machine.define(zero, dummy));
    cout << "define 1" << endl;
    require(state_machine.define(one, dummy));
    cout << "define 2" << endl;
    require(state_machine.define(two, dummy));
    cout << "fail define 3" << endl;
    require(!state_machine.define(3, dummy));
    cout << "fail define 4" << endl;
    require(!state_machine.define(4, dummy));
    cout << "current is zero" << endl;
    require(state_machine.getCurrentId() == zero);
    cout << "test No calls" << endl;
    require(dummy_call_count == 0);
    return EXIT_SUCCESS;
}
