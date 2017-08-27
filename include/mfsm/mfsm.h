#ifndef MFSM_H
#define MFSM_H


#include <type_traits>
#include <functional>
#include <array>
#include <tuple>
#include <variant>

#include "mfsm/meta.h"
#include "mfsm/lambda_overloading.h"


template <typename T>
struct TD;

namespace mfsm
{
    template <typename FsmDef>
    class fsm_base
    {
    private:
        template <typename F, typename... State>
        auto calc_ret_sum_type_impl(F && f, meta::type_list<State...>)
        {
            return meta::id<
                        meta::rename_template_t<
                                meta::map_t<
                                        meta::void_to_void_,
                                        meta::type_list<decltype(f(State{}))...>
                                >,
                                std::variant
                        >
                   >{};
        }

        template <typename StateEnterDef, typename... StateDef>
        auto calc_ret_sum_type(StateEnterDef && startState, StateDef &&... state)
        {          
            return calc_ret_sum_type_impl(
                        util::make_overload(
                                [ss = startState](meta::void_) { return ss(); },
                                state...
                        ),
                        typename FsmDef::state_list_t{}
                   );
        }

    protected:
        template <typename StateEnterDef, typename... StateDef>
        auto build_fsm(StateEnterDef && startState, StateDef &&... state)
        {
            auto o = util::make_overload(
                            [ss = std::forward<StateEnterDef>(startState)](meta::void_) { return ss(); },
                            std::forward<StateDef>(state)...
                     );

            using state_sum_t = typename FsmDef::state_sum_t;
            using ret_sum_t = typename decltype(calc_ret_sum_type(startState, state...))::type;

            //TD<ret_sum_t> td;

            return [o](state_sum_t ss) -> ret_sum_t {
                return std::visit(
                            [&o](auto s) {
                                if constexpr (std::is_same_v<decltype(o(s)), void>) {
                                    return ret_sum_t{ meta::void_{} };
                                }
                                else {
                                    return ret_sum_t{ o(s) };
                                }
                            },
                            ss
                       );
            };
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
    using state_sum_t = std::variant<mfsm::meta::void_, Init, Read, Process, Exit>;

    using action_func_sig_t = std::function<state_sum_t(state_sum_t)>;

public:
    using mfsm::fsm_base<user_input_echo>::operator ();

public:
    action_func_sig_t const & get_action_func() const
    { return actionFunc_; }

private:
    action_func_sig_t const actionFunc_ = build_fsm(
        [](void)    -> Init                         { return Init{}; },
        [](Init)    -> Read                         { return Read{}; },
        [](Read)    -> std::variant<Process, Exit>  { return Process{}; },
        [](Process) -> Read                         { return Read{}; },
        [](Exit)    -> void                         { }
    );
};


#endif // MFSM_H


/*
'TD<
        std::__1::variant<
                    user_input_echo::Init,
                    user_input_echo::Read,
                    std::__1::variant<user_input_echo::Process, user_input_echo::Exit>,
                    user_input_echo::Read,
                    mfsm::meta::void_
        >
>'
*/


/*
no viable conversion

from
'(lambda at /Users/gilhojang/GitHub/mfsm/test/../include/mfsm/mfsm.h:75:20)'

to 'const action_func_sig_t' (aka 'const function<
                                            variant<
                                                mfsm::meta::void_,
                                                user_input_echo::Init,
                                                user_input_echo::Read,
                                                user_input_echo::Process,
                                                user_input_echo::Exit
                                            >
                                                (variant<
                                                    mfsm::meta::void_,
                                                    user_input_echo::Init,
                                                    user_input_echo::Read,
                                                    user_input_echo::Process,
                                                    user_input_echo::Exit
                                                 >)
                                   >')
*/