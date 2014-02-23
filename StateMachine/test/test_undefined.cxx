#include <cstddef>
#include <cstdint>
#include <iostream>
#include "require.h"
#include <StateMachine.h>

int main(int argc, char* argv[]) {
    using namespace std;
    StateMachine state_machine(5);
    cout << "isdefined 0 - 6 test" << endl;
    require(!state_machine.isDefined(0));
    require(!state_machine.isDefined(1));
    require(!state_machine.isDefined(2));
    require(!state_machine.isDefined(4));
    require(!state_machine.isDefined(5));
    require(!state_machine.isDefined(6));
    cout << "nothing service test" << endl;
    state_machine.service(); // should do nothing
    cout << "nothing next test" << endl;
    require(state_machine.next() == 0);
    cout << "noting prev test" << endl;
    require(state_machine.prev() == 0);
    cout << "done" << endl;
    return EXIT_SUCCESS;
}
