#ifndef MFSM_HELPER_MACRO_H
#define MFSM_HELPER_MACRO_H


#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>


#define MFSM_print_type(r, data, elem)                          \
    struct elem { };                                            \


#define DEFINE_STATE_TYPE(...)                                  \
    public:                                                     \
        BOOST_PP_SEQ_FOR_EACH(                                  \
            MFSM_print_type,                                    \
            ~,                                                  \
            BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)               \
        )                                                       \
                                                                \
        using state_list_t = skull::prelude::TL<                \
                                    mfsm::meta::void_,          \
                                    __VA_ARGS__                 \
                             >;                                 \


#define DEFINE_STATE_TRANSITION_TABLE(...)                      \
    public:                                                     \
        static auto const & get_action_func()                   \
        { return actionFunc_; }                                 \
                                                                \
    private:                                                    \
        inline static auto const actionFunc_ = build_fsm(       \
            __VA_ARGS__                                         \
        );                                                      \


#endif // MFSM_HELPER_MACRO_H
