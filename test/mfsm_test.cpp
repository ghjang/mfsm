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

template <typename... T>
struct type_list
{ };


template <typename... Args, typename L>
std::function<void(Args...)> to_function(L l)
{
    return l;
}

template <typename F>
struct extract_arg_type_list;

template <typename R, typename... Args>
struct extract_arg_type_list<std::function<R(Args...)>>
{
    using type = type_list<Args...>;
};

template <typename L>
auto arg_type_list_of_impl(L l)
{
    return typename extract_arg_type_list<
                        decltype(to_function(l))
                    >::type{};
}

template <typename L>
auto arg_type_list_of(L l)
{
    return arg_type_list_of_impl(l);
}


template <typename... Args, typename L>
std::function<void(Args...)> return_type_of_impl(L l)
{
    return l;
}

template <typename L>
auto return_type_of(L l)
{
    return id<typename decltype(return_type_of_impl(l))::result_type>{};
}


TEST_CASE("getting the argument type list of lambda expression function", "[mfsm]")
{
    // compile error: lambda expression in an unevaluated operand
    /*
    static_assert(
        std::is_same_v<
                decltype(arg_type_list_of([]{})),
                type_list<>
        >
    );
    */

    auto tl0 = arg_type_list_of([]{});
    static_assert(
        std::is_same_v<
                decltype(tl0),
                type_list<>
        >
    );

    // NOTE: it seems that we can't use std::function to get the argument type list.
    auto tl1 = arg_type_list_of([](int){});
    static_assert(
        std::is_same_v<
        decltype(tl1),
        type_list<>
        >
    );
}


TEST_CASE("getting the return type of lambda expression function", "[mfsm]")
{
    // compile error: lambda expression in an unevaluated operand
    //decltype([](int i){ return i; }) * pL;
    
    // This is OK.
    auto l = [](int i){ return i; };
    decltype(l) * pL = &l;
    CHECK((*pL)(10) == 10);
    
    return_type_of([]{});
    return_type_of([]{ return 10; });
    return_type_of_impl([]{ return 10; });
    
    // NOTE: it seems that we can't use std::function to get the return type.
}
