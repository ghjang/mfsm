#ifndef MFSM_META_H
#define MFSM_META_H


namespace mfsm::meta
{
    struct void_
    { };

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
} // namespace mfsm::meta


#endif // MFSM_META_H
