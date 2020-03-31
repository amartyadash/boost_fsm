#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/event.hpp>
#include <iostream>

namespace sc = boost::statechart;

//using structs to avoid having to write public everytime 

//We need to forward-declare the initial state because it can
// only be defined at a point where the state machine is
// defined.
struct Greeting;

//make state machine using CRTP .
//can give any name to the state machine.
struct Machine : sc::state_machine< Machine, Greeting> {};


//defining Greeting state:

struct Greeting : sc::simple_state< Greeting, Machine> {
    Greeting() {
        std::cout << "First state entered\n";
    }
    ~Greeting() {
        std::cout << "First state exited\n";
        
    }
};


int main() {
    Machine machine;
    machine.initiate();
    return 0;

}