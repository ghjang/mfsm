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
public:
    struct Init { };
    struct Read { };
    struct Process { };
    struct Exit { };

    using state_list_t = skull::prelude::TL<mfsm::meta::void_, Init, Read, Process, Exit>;

public:
    static auto const & get_action_func()
    { return actionFunc_; }

private:
    inline static auto const actionFunc_ = build_fsm(
        [](void)    -> Init                         { return Init{}; },
        [](Init)    -> Read                         { init(); return Read{}; },
        [](Read)    -> std::variant<Process, Exit>  { if (read()) return Process{}; else return Exit{}; },
        [](Process) -> Read                         { process(); return Read{}; },
        [](Exit)    -> void                         { exit(); }
    );
};


TEST_CASE("user input echo fsm", "[example]")
{
    user_input_echo fsm;
    
    fsm();
}