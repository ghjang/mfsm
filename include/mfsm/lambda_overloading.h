#ifndef MFSM_LAMBDA_OVERLOADING_
#define MFSM_LAMBDA_OVERLOADING_


namespace mfsm::util
{
#if __clang_major__ >= 5

    template<typename... T>
    struct overload : T...
    {
        overload(T... t)
            : T{ t }...
        { }

        using T::operator()...;
    };

    template <typename... T> overload(T...) -> overload<T...>;


    template <typename... LambdaExpr>
    auto make_overload(LambdaExpr... l)
    {
        return overload(l...);
    }

#else

    template<typename x, typename... xs>
    struct overload : x, overload<xs...>
    {
        overload(x head, xs... tail)
            : x(head), overload<xs...>(tail...)
        { }

        using x::operator();
        using overload<xs...>::operator();
    };

    template<typename x>
    struct overload<x> : x
    {
        overload(x head)
            : x(head)
        { }

        using x::operator();
    };


    template <typename... LambdaExpr>
    auto make_overload(LambdaExpr... l)
    {
        return overload<LambdaExpr...>(l...);
    }

# endif // __clang_major__ >= 5

} // namespace mfsm::util


#endif // MFSM_LAMBDA_OVERLOADING_
