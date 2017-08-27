#ifndef MFSM_META_H
#define MFSM_META_H


namespace mfsm::meta
{
    struct void_
    { };

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

    template <typename TypeList>
    inline constexpr auto length_v = length<TypeList>::value;


    template <typename T>
    struct id
    {
        using type = T;
    };
} // namespace mfsm::meta


#endif // MFSM_META_H
