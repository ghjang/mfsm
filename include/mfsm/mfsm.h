#ifndef MFSM_H
#define MFSM_H


#include <type_traits>
#include <functional>
#include <array>
#include <variant>


namespace mfsm
{
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
    public:
        auto operator () ()
        {
            //auto const & id2ActionMap = static_cast<FsmDef *>(this)->get_state_id_to_action_map();
        }

    protected:
        template <typename StateEnter, typename... StateDef>
        static auto calc_action_func_sig(StateEnter && startState, StateDef &&... defs)
        {
            using elem_t = std::variant<
                                std::function<void()>,
                                std::function<typename FsmDef::Read(typename FsmDef::Init)>,
                                std::function<std::variant<typename FsmDef::Process, typename FsmDef::Exit>(typename FsmDef::Read)>,
                                std::function<typename FsmDef::Read(typename FsmDef::Process)>,
                                std::function<void(typename FsmDef::Exit)>
                            >;
            return elem_t{};
        }

        template <typename StateEnter, typename... StateDef>
        auto build_fsm(StateEnter && startState, StateDef &&... defs)
        {
            using state_id_to_action_map_t = typename FsmDef::state_id_to_action_map_t;
            
            state_id_to_action_map_t id2ActionMap{
                std::forward<StateEnter>(startState),
                std::forward<StateDef>(defs)...
            };

            return id2ActionMap;
        }
    };


    struct user_input_echo : fsm_base<user_input_echo>
    {
    public:
        struct Init { };
        struct Read { };
        struct Process { };
        struct Exit { };

        using state_list_t = type_list<Init, Read, Process, Exit>;
        
    private:
        inline static auto const action_func_sig_val_ =
                                    calc_action_func_sig(
                                        []()        -> Init                         { return Init{}; },
                                        [](Init)    -> Read                         { return Read{}; },
                                        [](Read)    -> std::variant<Process, Exit>  { return Process{}; },
                                        [](Process) -> Read                         { return Read{}; },
                                        [](Exit)    -> void                         { }
                                    );
        
    public:
        using action_func_sig_t = std::decay_t<decltype(action_func_sig_val_)>;
        using state_id_to_action_map_t = std::array<action_func_sig_t, 1 + length<state_list_t>::value>;

    public:
        using fsm_base<user_input_echo>::operator ();

    public:
        state_id_to_action_map_t const & get_state_id_to_action_map() const
        { return id2Action_; }

    private:
        state_id_to_action_map_t const id2Action_ = build_fsm(
            []()        -> Init                         { return Init{}; },
            [](Init)    -> Read                         { return Read{}; },
            [](Read)    -> std::variant<Process, Exit>  { return Process{}; },
            [](Process) -> Read                         { return Read{}; },
            [](Exit)    -> void                         { }
        );
    };
} // namespace mfsm


#endif // MFSM_H
