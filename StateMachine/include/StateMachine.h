#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#ifdef __AVR__
    #include <Arduino.h>
#else
    #include <cstddef>
    #include <cstdint>
#endif // __AVR__

class StateMachine;
typedef class StateMachine StateMachine;
typedef void (*StateFunc)(StateMachine* state_machine);

class StateMachine {
    private:
        /* Member Types */
        class StateInfo {
            private:
                /* Static Data Members */
                static const uint8_t emptyStateID = (uint8_t)-1;
                static constexpr StateFunc emptyStateFunc = NULL;
            public:
                /* Data Members */
                uint8_t stateID;
                StateFunc stateFunc;
                /* Member functions */
                // Default constructor
                StateInfo(uint8_t state_id=emptyStateID,
                          StateFunc state_func=emptyStateFunc);
                // return true if state_id & state_func represent empty
                bool isEmpty(const uint8_t state_id,
                             const StateFunc state_func) const;
                // return true if item is unset
                bool isEmpty();
                // set this instance to empty
                void makeEmpty(void);
        };
        /* Data Members */
        const uint8_t maxStates;
        uint8_t currentState;
        uint8_t smallest;
        uint8_t largest;
        uint8_t totalDefined;
        StateInfo** stateInfos;
        // return pointer to empty StateInfo or NULL if full
        StateInfo* nextEmpty(void);
    public:
        /* Member functions */
        // constructor, caller must define() to add states
        StateMachine(uint8_t max_states, uint8_t initial_state=0);
        // Destructor
        ~StateMachine();
        // Set handler function pointer for state, returns false on failure
        bool define(uint8_t state, StateFunc state_func);
        // Unset handler and declare state invalid for servicing
        void undefine(uint8_t state);
        // returns current state as an integer
        uint8_t getCurrentId(void) const;
        // sets state to new_state
        void setCurrentId(uint8_t new_state);
        // returns true if state is valid and has a handler function pointer
        bool isDefined(uint8_t state);
        // Call all function with current state, in order, nothing if undefined
        void service(void);
        // Set & return current state to next available larger state
        // undefined result if no states defined
        uint8_t next(void);
        // Set & return current state to prior available smaller state
        // undefined result if no states defined
        uint8_t prev(void);
        // Reutn nmber of states defined
        uint8_t defined(void) const;
};

#endif // STATEMACHINE_H
