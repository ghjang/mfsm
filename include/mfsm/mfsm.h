#ifndef MFSM_H
#define MFSM_H


#include <type_traits>
#include <functional>
#include <array>
#include <tuple>
#include <variant>

#include "mfsm/meta.h"
#include "mfsm/lambda_overloading.h"


namespace mfsm
{
    template <typename FsmDef>
    class fsm_base
    {
    private:
        template <typename F, typename... State>
        static auto calc_action_func_sig_impl(F && f, meta::type_list<State...>)
        {
            return meta::id<
                        std::variant<
                                std::function<decltype(f(State{}))(State)>...
                        >
                   >{};
        }

    protected:
        template <typename StateEnter, typename... StateDef>
        static auto calc_action_func_sig(StateEnter && startState, StateDef &&... defs)
        {          
            return calc_action_func_sig_impl(
                        mfsm::util::make_overload(
                                        [ss = std::forward<StateEnter>(startState)](meta::void_) { return ss(); },
                                        std::forward<StateDef>(defs)...
                        ),
                        typename FsmDef::state_list_t{}
                   );
        }

        template <typename StateEnter, typename... StateDef>
        auto build_fsm(StateEnter && startState, StateDef &&... defs)
        {
            using state_id_to_action_map_t = typename FsmDef::state_id_to_action_map_t;
            
            state_id_to_action_map_t id2ActionMap{
                [ss = std::forward<StateEnter>(startState)](meta::void_) { return ss(); },
                std::forward<StateDef>(defs)...
            };

            return id2ActionMap;
        }

    public:
        auto operator () ()
        {
            //auto const & id2ActionMap = static_cast<FsmDef *>(this)->get_state_id_to_action_map();
        }
    };
} // namespace mfsm


struct user_input_echo
            : mfsm::fsm_base<user_input_echo>
{
public:
    struct Init { };
    struct Read { };
    struct Process { };
    struct Exit { };

    using state_list_t = mfsm::meta::type_list<mfsm::meta::void_, Init, Read, Process, Exit>;
    
private:
    inline static auto const action_func_sig_val_ =
                                calc_action_func_sig(
                                    [](void)    -> Init                         { return Init{}; },
                                    [](Init)    -> Read                         { return Read{}; },
                                    [](Read)    -> std::variant<Process, Exit>  { return Process{}; },
                                    [](Process) -> Read                         { return Read{}; },
                                    [](Exit)    -> void                         { }
                                );
    
public:
    using action_func_sig_t = decltype(action_func_sig_val_)::type;
    using state_id_to_action_map_t = std::array<action_func_sig_t, mfsm::meta::length_v<state_list_t>>;

public:
    using fsm_base<user_input_echo>::operator ();

public:
    state_id_to_action_map_t const & get_state_id_to_action_map() const
    { return id2Action_; }

private:
    state_id_to_action_map_t const id2Action_ = build_fsm(
        [](void)    -> Init                         { return Init{}; },
        [](Init)    -> Read                         { return Read{}; },
        [](Read)    -> std::variant<Process, Exit>  { return Process{}; },
        [](Process) -> Read                         { return Read{}; },
        [](Exit)    -> void                         { }
    );
};


#endif // MFSM_H
