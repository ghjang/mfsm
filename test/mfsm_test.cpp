#include "catch.hpp"

#include <type_traits>
#include <functional>


TEST_CASE("std::function", "[mfsm]")
{
    std::function<void()> f0 = []{ return 10; };
    f0();

    std::function<void(int)> f1 = [](int i){ return i; };
    f1(10);

    // compile error: invalid operands to binary expression ('void' and 'int')
    //f1(10) == 10;

    static_assert(
        std::is_same_v<
                decltype(f1(10)),
                void
        >
    );
}


template <typename T>
struct id
{
    using type = T;
};

template <typename... Args, typename L>
std::function<void(Args...)> return_type_of_impl(L l)
{
    return l;
}

template <typename R, typename L>
id<R> return_type_of(L l)
{
    return id<typename decltype(return_type_of_impl(l))::result_type>{};
}

TEST_CASE("getting the return type of lambda expression function", "[mfsm]")
{
    // compile error: lambda expression in an unevaluated operand
    //decltype([](int i){ return i; }) * pL;

    // This is OK.
    auto l = [](int i){ return i; };
    decltype(l) * pL = &l;
    CHECK((*pL)(10) == 10);

    // compile error: couldn't infer template argument 'R'
    //return_type_of([]{});
    //return_type_of([]{ return 10; });
    return_type_of_impl([]{ return 10; });
}
