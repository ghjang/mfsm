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


    template <template <typename> class F, typename TypeList>
    struct map;

    template <template <typename> class F, typename... T>
    struct map<F, type_list<T...>>
    {
        using type = type_list<typename F<T>::type...>;
    };

    template <template <typename> class F, typename TypeList>
    using map_t = typename map<F, TypeList>::type;


    template <typename T>
    struct void_to_void_
    {
        using type = T;
    };

    template <>
    struct void_to_void_<void>
    {
        using type = meta::void_;
    };


    template <typename S, template <typename...> class T>
    struct rename_template;

    template <template <typename...> class S, typename... E, template <typename...> class T>
    struct rename_template<S<E...>, T>
    {
        using type = T<E...>;
    };

    template <typename S, template <typename...> class T>
    using rename_template_t = typename rename_template<S, T>::type;
} // namespace mfsm::meta


#endif // MFSM_META_H
