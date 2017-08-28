#include "catch.hpp"

#include "mfsm/mfsm.h"

#include <string>
#include <iostream>


namespace
{
    std::string g_inputLine;

    void init() { std::cout << "Hello~!\n"; }

    bool read()
    {
        std::cout << "input: "; std::getline(std::cin, g_inputLine);
        return !g_inputLine.empty();
    }

    void process() { std::cout << "output: " << g_inputLine << '\n'; }    
    void exit() { std::cout << "Good Bye!." << std::endl; }
} // anonymous namespace


struct user_input_echo
            : mfsm::fsm_base<user_input_echo>
{
    DEFINE_STATE_TYPE(Init, Read, Process, Exit)

    DEFINE_STATE_TRANSITION_TABLE(
        [](void)    -> Init                         {
                                                        return Init{};  // enter Init state.
                                                    },
        [](Init)    -> Read                         {
                                                        init();         // do the initialization.
                                                        return Read{};  // move to Read state.
                                                    },
        [](Read)    -> std::variant<Process, Exit>  {
                                                        if (read())             // if read some data,
                                                            return Process{};   // move to Process state.
                                                        else                    // or else,
                                                            return Exit{};      // move to Exit state.
                                                    },
        [](Process) -> Read                         {
                                                        process();      // process the data.
                                                        return Read{};  // go back to Read state.
                                                    },
        [](Exit)    -> void                         {
                                                        exit();         // exit this FSM execution.
                                                    }
    )
};


TEST_CASE("user input echo fsm", "[example]")
{
    user_input_echo fsm;
    
    fsm();
}
