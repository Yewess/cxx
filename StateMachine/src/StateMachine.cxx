#ifdef __AVR__
    #include <stdint.h>
    #include <Arduino.h>
#else
    #include <cstddef>
    #include <cstdint>
#endif // __AVR__

#include "StateMachine.h"

// public

// Default constructor
inline StateMachine::StateInfo::StateInfo(uint8_t state_id,
                                          StateFunc state_func)
                                          :
                                          stateID(state_id),
                                          stateFunc(state_func) {}

// return true if state_id & state_func represent empty
inline bool StateMachine::StateInfo::isEmpty(const uint8_t state_id,
                                             const StateFunc state_func)
                                             const {
    return ((state_id == emptyStateID) || (state_func == emptyStateFunc));
}

// return true if item is unset
inline bool StateMachine::StateInfo::isEmpty() {
    return isEmpty(stateID, stateFunc);
}

// set this instance to empty
inline void StateMachine::StateInfo::makeEmpty(void) {
    stateID = emptyStateID;
    stateFunc = emptyStateFunc;
}

// Private
// return pointer to empty StateInfo or NULL if full
StateMachine::StateInfo* StateMachine::nextEmpty(void) {
    for (uint8_t index=0; index < maxStates; index++)
        if (stateInfos[index]->isEmpty())
            return stateInfos[index];
    return NULL;
}

// Public
// constructor, caller must define() to add states
StateMachine::StateMachine(const uint8_t max_states,
                           const uint8_t initial_state)
                           :
                           maxStates(max_states),
                           currentState(initial_state),
                           smallest(max_states),
                           largest(0),
                           totalDefined(0) {
    stateInfos = new StateMachine::StateInfo*[max_states];
    for (uint8_t index=0; index < maxStates; index++)
        stateInfos[index] = new StateMachine::StateInfo;
}

// Destructor
StateMachine::~StateMachine() {
    if (stateInfos != NULL) {
        for (uint8_t index=0; index < maxStates; index++) {
            if (stateInfos[index] != NULL) {
                delete stateInfos[index];
                stateInfos[index] = NULL;
            }
        }
        delete stateInfos;
        stateInfos = NULL;
    }
    totalDefined = 0;
    largest = 0;
    smallest = 0;
    currentState = 0;
}

// Set handler function pointer for state, returns false on failure
bool StateMachine::define(uint8_t state, StateFunc state_func) {
    if (!(totalDefined >= maxStates)) {
        StateMachine::StateInfo* state_info = nextEmpty();
        if (state_info != NULL)
            if (!state_info->isEmpty(state, state_func)) {
                if (state < smallest)
                    smallest = state;
                if (state > largest)
                    largest = state;
                state_info->stateID = state;
                state_info->stateFunc = state_func;
                totalDefined++;
                return true;
            }
    }
    return false;
}

// Unset handler and declare state invalid for servicing
void StateMachine::undefine(uint8_t state) {
    if (totalDefined > 0) {
        for (uint8_t index=0; index < maxStates; index++) {
            if (stateInfos[index]->stateID == state) {
                stateInfos[index]->makeEmpty();
                totalDefined--;
            }
        }
    }
    if (totalDefined > 0) {
        smallest = maxStates;
        largest = 0;
        for (uint8_t index=0; index < maxStates; index++) {
            if (!stateInfos[index]->isEmpty()) {
                if (stateInfos[index]->stateID > largest)
                    largest = stateInfos[index]->stateID;
                if (stateInfos[index]->stateID < smallest)
                    smallest = stateInfos[index]->stateID;
            }
        }
    }
}

// returns current state as an integer
uint8_t StateMachine::getCurrentId(void) const {
    return currentState;
}

// sets state to new_state
void StateMachine::setCurrentId(uint8_t new_state) {
    currentState = new_state;
}

// returns true if state is valid and has a handler function pointer
bool StateMachine::isDefined(uint8_t state) {
    for (uint8_t index=0; index < maxStates; index++) {
        if (stateInfos[index]->stateID == state) {
            return true;
        }
    }
    return false;
}

// Call all function with current state, in order, nothing if undefined
void StateMachine::service(void) {
    if (totalDefined == 0)
        return;
    uint8_t servicing = currentState;
    for (uint8_t index=0; index < maxStates; index++)
        if (stateInfos[index]->stateID == servicing)
            if (stateInfos[index]->stateFunc != NULL)
                (stateInfos[index]->stateFunc)(this);
}

// Set & return next available larger state
uint8_t StateMachine::next(void) {
    if (totalDefined == 0)
        return 0;
    if (currentState >= largest)
        return currentState = smallest;
    uint16_t smallestDelta = (uint16_t)-1;
    uint8_t deltaIndex = currentState;
    for (uint8_t index=0; index < maxStates; index++) {
        if (!stateInfos[index]->isEmpty()) {
            // Duplicate stateID's called together in service
            if (stateInfos[index]->stateID <= currentState)
                continue;
            if (stateInfos[index]->stateID - currentState < smallestDelta) {
                smallestDelta = stateInfos[index]->stateID - currentState;
                deltaIndex = index;
            }
        }
    }
    return currentState = stateInfos[deltaIndex]->stateID;
}

// Set & return prior available smaller state
uint8_t StateMachine::prev(void) {
    if (totalDefined == 0)
        return 0;
    if (currentState <= smallest)
        return currentState = largest;
    uint16_t largestDelta = 0;
    uint8_t deltaIndex = currentState;
    for (uint8_t index=(maxStates-1); index > 0; index--) {
        if (!stateInfos[index]->isEmpty()) {
            // Duplicate stateID's called together in service
            if (stateInfos[index]->stateID >= currentState)
                continue;
            if (currentState - stateInfos[index]->stateID > largestDelta) {
                largestDelta = currentState - stateInfos[index]->stateID;
                deltaIndex = index;
            }
        }
    }
    return currentState = deltaIndex;
}

uint8_t StateMachine::defined(void) const {
    //return (sizeof(StateInfo) * maxStates) +
    //       (sizeof(StateInfo*) * maxStates) +
    //       (sizeof(StateMachine));
    return totalDefined;
}
