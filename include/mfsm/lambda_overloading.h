#ifndef MFSM_LAMBDA_OVERLOADING_
#define MFSM_LAMBDA_OVERLOADING_


namespace mfsm::util
{
    template<class... T>
    struct overloaded : T...
    {
        using T::operator()...;
    };

    template<class... T> overloaded(T...) -> overloaded<T...>;


    template<typename x, typename... xs>
    struct overload : x, overload<xs...>
    {
        overload(x head, xs... tail)
            : x(head), overload<xs...>(tail...)
        { }

        using x::operator();
        using overload<xs...>::operator();
    };

    template<class x>
    struct overload<x> : x
    {
        overload(x head)
            : x(head)
        { }

        using x::operator();
    };


    template <class... LambdaExpr>
    auto make_overload(LambdaExpr... l)
    {
        return overload<LambdaExpr...>(l...);
    }

} // namespace mfsm::util


#endif // MFSM_LAMBDA_OVERLOADING_
