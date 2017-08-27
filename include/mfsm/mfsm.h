#ifndef MFSM_H
#define MFSM_H


#include <type_traits>
#include <functional>
#include <array>
#include <tuple>
#include <variant>

#include "mfsm/lambda_overloading.h"


namespace mfsm
{
    struct void_
    { };

    template <typename T>
    struct id
    {
        using type = T;
    };

    template <typename... T>
    struct type_list
    { };

    template <typename TypeList>
    struct length;

    template <typename... T>
    struct length<type_list<T...>>
    {
        static constexpr auto value = sizeof...(T);
    };


    template <typename FsmDef>
    class fsm_base
    {
    private:
        template <typename F, typename... State>
        static auto calc_action_func_sig_impl(F && f, type_list<State...>)
        {
            return id<
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
                                        [ss = std::forward<StateEnter>(startState)](void_) { return ss(); },
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
                [ss = std::forward<StateEnter>(startState)](void_) { return ss(); },
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


    struct user_input_echo : fsm_base<user_input_echo>
    {
    public:
        struct Init { };
        struct Read { };
        struct Process { };
        struct Exit { };

        using state_list_t = type_list<void_, Init, Read, Process, Exit>;
        
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
        using state_id_to_action_map_t = std::array<action_func_sig_t, length<state_list_t>::value>;

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
} // namespace mfsm


#endif // MFSM_H
