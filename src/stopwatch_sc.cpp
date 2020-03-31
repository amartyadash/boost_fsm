#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/transition.hpp>
#include <iostream>
#include <ctime>

namespace sc = boost::statechart;
struct EvStartStop : sc::event< EvStartStop > {};
struct EvReset : sc::event< EvReset > {};

struct IElapsedTime {
    virtual double elapsedTime() const = 0;
};

struct Active;
struct Stopped;
struct Running;

struct StopWatchMachine : sc::state_machine < StopWatchMachine, Active > {
    double elapsedTime() const {
        return state_cast< const IElapsedTime & >().elapsedTime();
    }
};

struct Active : sc::simple_state < Active, StopWatchMachine, Stopped > {
    public:
        typedef sc::transition< EvReset, Active > reactions;

        Active(): elapsed_time_(0.0) {} //contructor
        
        double elapsedTime() const { 
            return elapsed_time_;
        }
        
        double & elapsedTime() {
            return elapsed_time_;
        }
    
    private:
        double elapsed_time_;
};

struct Running : IElapsedTime, sc::simple_state <Running, Active> {
    public:
        typedef sc::transition< EvStartStop, Stopped > reactions;
    
        Running() : start_time_(std::time(0)) {
            std::cout << "Running" << std::endl;
        }

        ~Running() {
            context<Active>().elapsedTime() = elapsedTime();
            std::cout << "Stopped" << std::endl;

            
        }
        virtual double elapsedTime() const {
            return context< Active >().elapsedTime() + std::difftime( std::time( 0 ), start_time_ );
        }


    private:
        std::time_t start_time_;
};

struct Stopped : IElapsedTime, sc::simple_state <Stopped, Active> {
    
    typedef sc::transition< EvStartStop, Running > reactions;
    
    virtual double elapsedTime() const {
        return context< Active >().elapsedTime();
    }
};

int main() {
    StopWatchMachine watch;
    watch.initiate();
    
    std::cout << "Time right now:" << watch.elapsedTime() << std::endl; //starts the watch, is stopped right now

    watch.process_event(EvStartStop()); //turn it on
    sleep(4); // wait 4 seconds


    watch.process_event(EvStartStop()); //off

    std::cout << "Time right now:" << watch.elapsedTime() << std::endl; 
    
    watch.process_event(EvStartStop()); //on
    sleep(3);
    watch.process_event(EvStartStop());
    std::cout << "Time right now:" << watch.elapsedTime() << std::endl;

    watch.process_event(EvReset()); // reset
    std::cout << "Reset" << std::endl;
    std::cout << "Time right now:" << watch.elapsedTime() << std::endl; //now set to 0 again

    return 0;
}
