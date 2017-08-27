#ifndef MFSM_LAMBDA_OVERLOADING_
#define MFSM_LAMBDA_OVERLOADING_


namespace mfsm::util
{
    // NOTE: this feature is not implemented in LLVM 4.0.0.
    /*
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
    */


    template<typename L, typename... Ls>
    struct overload : L, overload<Ls...>
    {
        overload(L l, Ls... ls)
            : L(l), overload<Ls...>(ls...)
        { }

        using L::operator();
        using overload<Ls...>::operator();
    };

    template<class L>
    struct overload<L> : L
    {
        overload(L l)
            : L(l)
        { }

        using L::operator();
    };


    template <class... L>
    auto make_overload(L... l)
    {
        return overload<L...>(l...);
    }

} // namespace mfsm::util


#endif // MFSM_LAMBDA_OVERLOADING_
