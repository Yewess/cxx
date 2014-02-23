#include <cstddef>
#include <cstdint>
#include <iostream>
#include "require.h"
#include <StateMachine.h>

int main(int argc, char* argv[]) {
    using namespace std;
    StateMachine state_machine(5);
    require(state_machine.getCurrentId() == 0);
    return EXIT_SUCCESS;
}
