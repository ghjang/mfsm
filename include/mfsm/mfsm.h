#ifndef MFSM_H
#define MFSM_H


#include <type_traits>
#include <functional>
#include <variant>

#include "skull/prelude.h"
#include "skull/app.h"

#include "mfsm/meta.h"
#include "mfsm/lambda_overloading.h"


namespace mfsm
{
    template <typename FsmDef>
    class fsm_base
    {
    private: 
        template <typename F, typename... State>
        static auto calc_ret_sum_type_impl(F && f, skull::prelude::TL<State...>)
        {
            using namespace skull::prelude;
            using skull::app::unique_t;

            return id<
                        rename_template_t<
                            unique_t<
                                map_t<
                                    quote<meta::void_to_void_>,
                                    TL<decltype(f(State{}))...>
                                >
                            >,
                            std::variant
                        >
                   >{};
        }

    protected:
        template <typename StateEnterDef, typename... StateDef>
        static auto calc_ret_sum_type(StateEnterDef && startState, StateDef &&... state)
        {          
            return calc_ret_sum_type_impl(
                        util::make_overload(
                                [ss = startState](meta::void_) { return ss(); },
                                state...
                        ),
                        typename FsmDef::state_list_t{}
                   );
        }

        template <typename StateEnterDef, typename... StateDef>
        auto build_fsm(StateEnterDef && startState, StateDef &&... state)
        {
            auto o = util::make_overload(
                            [ss = std::forward<StateEnterDef>(startState)](meta::void_) { return ss(); },
                            std::forward<StateDef>(state)...
                     );

            using arg_sum_t = typename FsmDef::arg_sum_t;
            using ret_sum_t = typename FsmDef::ret_sum_t;

            return [o](arg_sum_t as) -> ret_sum_t {
                return std::visit(
                            [&o](auto s) {
                                if constexpr (std::is_same_v<decltype(o(s)), void>) {
                                    o(s);
                                    return ret_sum_t{ meta::void_{} };
                                }
                                else {
                                    return ret_sum_t{ o(s) };
                                }
                            },
                            as
                       );
            };
        }

    public:
        auto operator () ()
        {
            using arg_sum_t = typename FsmDef::arg_sum_t;
            using ret_sum_t = typename FsmDef::ret_sum_t;

            bool isExit = false;
            auto const & actionFunc = static_cast<FsmDef *>(this)->get_action_func();
            arg_sum_t arg = meta::void_{};  // make it enter the initial state.
            ret_sum_t ret;

            do {
                ret = actionFunc(arg);
                std::visit(
                    [&isExit, &arg](auto r) {
                        if constexpr (std::is_same_v<decltype(r), meta::void_>) {
                            // if the terminal state is reached,
                            isExit = true;
                        }
                        else if constexpr (skull::prelude::is_same_template_v<decltype(r), std::variant<>>) {
                            // if the next possible transition states are multiple,
                            //      TODO: if nested states also have multiple states successively? recursion?
                            std::visit(
                                [&isExit, &arg](auto rr) {
                                    if constexpr (std::is_same_v<decltype(rr), meta::void_>) {
                                        isExit = true;
                                    }
                                    else {
                                        arg = rr;
                                    }
                                },
                                r
                            );
                        }
                        else {
                            arg = r;
                        }
                    },
                    ret
                );
            }
            while (!isExit);
        }
    };
} // namespace mfsm


#endif // MFSM_H
