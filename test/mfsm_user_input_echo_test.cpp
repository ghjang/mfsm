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

private:
    inline static auto const ret_sum_t_val_ = mfsm::fsm_base<user_input_echo>::calc_ret_sum_type(
                                                    [](void)    -> Init                         { return Init{}; },
                                                    [](Init)    -> Read                         { return Read{}; },
                                                    [](Read)    -> std::variant<Process, Exit>  { return Process{}; },
                                                    [](Process) -> Read                         { return Read{}; },
                                                    [](Exit)    -> void                         { }
                                              );

public:
    using arg_sum_t = std::variant<mfsm::meta::void_, Init, Read, Process, Exit>;
    using ret_sum_t = typename decltype(ret_sum_t_val_)::type;

    using action_func_sig_t = std::function<ret_sum_t(arg_sum_t)>;

public:
    action_func_sig_t const & get_action_func() const
    { return actionFunc_; }

private:
    action_func_sig_t const actionFunc_ = build_fsm(
        [this](void)    -> Init                         { return Init{}; },
        [this](Init)    -> Read                         { init(); return Read{}; },
        [this](Read)    -> std::variant<Process, Exit>  { if (read()) return Process{}; else return Exit{}; },
        [this](Process) -> Read                         { process(); return Read{}; },
        [this](Exit)    -> void                         { exit(); }
    );
};


TEST_CASE("user input echo fsm", "[example]")
{
    user_input_echo fsm;
    
    fsm();
}
