#ifndef MFSM_HELPER_MACRO_H
#define MFSM_HELPER_MACRO_H


#define DEFINE_TRANSITION_TABLE(...)                            \
    public:                                                     \
        static auto const & get_action_func()                   \
        { return actionFunc_; }                                 \
                                                                \
    private:                                                    \
        inline static auto const actionFunc_ = build_fsm(       \
            __VA_ARGS__                                         \
        );                                                      \


#endif // MFSM_HELPER_MACRO_H
