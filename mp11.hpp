#ifndef MP11_HPP
#define MP11_HPP

// http://pdimov.com/cpp2/simple_cxx11_metaprogramming.html
// https://github.com/ericniebler/meta
// https://github.com/ldionne/mpl11

#include <type_traits>

namespace mp11 {

/**************************** Basic list structure. ***************************/

template<class... T> struct mp_list {};

/********************************** MISC. *************************************/

template<class T> using mp_add_pointer = T*;
template<class T> using mp_add_reference = T&;
template<class T> using mp_add_const = const T;

/***************************** FALSE metafunction. ****************************/

using mp_true = std::integral_constant<bool, true>;

/***************************** TRUE metafunction. *****************************/

using mp_false = std::integral_constant<bool, false>;

/****************************** BOOL metafunction. ****************************/

template<bool b>
using mp_bool = std::integral_constant<bool, b>;

/***************************** CLEAR metafunction. ****************************/

namespace detail {

template<class L> struct mp_clear_impl;

template<template<class...> class L, class... T>
struct mp_clear_impl<L<T...>>
{
    using type = L<>;
};

} // detail

template<class L>
using mp_clear = typename detail::mp_clear_impl<L>::type;

/***************************** EMPTY metafunction. ****************************/

namespace detail {

template<class T>
struct mp_empty_impl;

template<template<class...> class L>
struct mp_empty_impl<L<>> : mp_true {};

template<template<class...> class L, class T1, class... T>
struct mp_empty_impl<L<T1, T...>> : mp_false {};

} // detail

template<class T> using mp_empty = typename detail::mp_empty_impl<T>::type;

/***************************** FRONT metafunction. ****************************/

namespace detail {

template<class L>
struct mp_front_impl;

template<template<class...> class L, class T1, class... T>
struct mp_front_impl<L<T1, T...>>
{
    using type = T1;
};

} // detail

template<class L>
using mp_front = typename detail::mp_front_impl<L>::type;

/***************************** BACK metafunction. *****************************/

namespace detail {

template<class L>
struct mp_back_impl;

template<template<class...> class L, class T1>
struct mp_back_impl<L<T1>>
{
    using type = T1;
};

template<template<class...> class L, class T1, class... T>
struct mp_back_impl<L<T1, T...>>
{
    using type = typename mp_back_impl<L<T...>>::type;
};

} // detail

template<class L>
using mp_back = typename detail::mp_back_impl<L>::type;

/*************************** PUSH_FONT metafunction. **************************/

namespace detail {

template<class L, class T>
struct mp_push_front_impl;

template<template<class...> class L, class... U, class... T>
struct mp_push_front_impl<L<U...>, T...> {
    using type = L<T..., U...>;
};

} // detail

template<class L, class T>
using mp_push_front = typename detail::mp_push_front_impl<L, T>::type;

/*************************** PUSH_BACK metafunction. **************************/

namespace detail {

template<class L, class T>
struct mp_push_back_impl;

template<template<class...> class L, class... U, class... T>
struct mp_push_back_impl<L<U...>, T...> {
    using type = L<U..., T...>;
};

} // detail

template<class L, class T>
using mp_push_back = typename detail::mp_push_back_impl<L, T>::type;

/*************************** POP_FRONT metafunction. **************************/

namespace detail {

template<class L>
struct mp_pop_front_impl;

template<template<class...> class L>
struct mp_pop_front_impl<L<>>
{
    using type = L<>;
};

template<template<class...> class L, class T1, class... T>
struct mp_pop_front_impl<L<T1, T...>>
{
    using type = L<T...>;
};

} // detail

template<class L>
using mp_pop_front = typename detail::mp_pop_front_impl<L>::type;

/*************************** POP_BACK metafunction. ***************************/

namespace detail {

template<class E, class... T>
struct mp_pop_back_impl;

template<class L>
struct mp_pop_back_impl2;

template<template<class...> class L, class... T>
struct mp_pop_back_impl2<L<T...>> {
    using type = typename mp_pop_back_impl<mp_empty<mp_list<T...>>, L<T...>>::type;
};

template<template<class...> class L, class T1>
struct mp_pop_back_impl<mp_false, L<T1>> {
    using type = L<>;
};

template<template<class...> class L, class... T>
struct mp_pop_back_impl<mp_false, L<T...>> {
    using _first = mp_front<L<T...>>;
    using _rest = typename mp_pop_back_impl2<mp_pop_front<L<T...>>>::type;

    using type = mp_push_front<_rest, _first>;
};

template<template<class...> class L, class... T>
struct mp_pop_back_impl<mp_true, L<T...>> {
    using type = L<>;
};

} // detail

template<class L>
using mp_pop_back = typename detail::mp_pop_back_impl2<L>::type;

/**************************** RENAME metafunction. ****************************/

namespace detail {

template<class A, template<class...> class B>
struct mp_rename_impl;

template<template<class...> class A, class... T, template<class...> class B>
struct mp_rename_impl<A<T...>, B>
{
    using type = B<T...>;
};

} // detail

template<class A, template<class...> class B>
using mp_rename = typename detail::mp_rename_impl<A, B>::type;

/**************************** LENGTH metafunction. ****************************/

template<class... T>
using mp_length = std::integral_constant<std::size_t, sizeof...(T)>;

/***************************** SIZE metafunction. *****************************/

template<class T>
using mp_size = mp_rename<T, mp_length>;

/***************************** TRANSFORM metafunction. *****************************/

namespace detail {

template<template<class...> class F, class... L>
struct mp_transform_impl;

template<template<class...> class F, template<class...> class L, class... T>
struct mp_transform_impl<F, L<T...>>
{
    using type = L<F<T>...>;
};

template<
    template<class...> class F,
    template<class...> class L1, class... T1,
    template<class...> class L2, class... T2>
struct mp_transform_impl<F, L1<T1...>, L2<T2...>>
{
    static_assert(sizeof...(T1) == sizeof...(T2), "");
    using type = L1<F<T1, T2>...>;
};

} // detail

template<template<class...> class F, class... L>
using mp_transform = typename detail::mp_transform_impl<F, L...>::type;

/***************************** PLUS metafunction. *****************************/

namespace detail {

template<class... T>
struct mp_plus_impl;

template<class... T>
using mp_plus_helper = typename mp_plus_impl<T...>::type;

template<> struct mp_plus_impl<> {
    using type = std::integral_constant<int, 0>;
};

template<class T1, class... T>
struct mp_plus_impl<T1, T...> {
    static constexpr auto _v = T1::value + mp_plus_helper<T...>::value;
    using type = std::integral_constant<typename std::decay<decltype(_v)>::type, _v>;
};

} // detail

template<class... T>
using mp_plus = detail::mp_plus_helper<T...>;

/**************************** COUNT metafunction. *****************************/

namespace detail {

template<class L, class V>
struct mp_count_impl;

template<template<class...> class L, class... T, class V>
struct mp_count_impl<L<T...>, V> {
    using type = mp_plus<std::is_same<T, V>...>;
};

} // detail

template<class L, class V>
using mp_count = typename detail::mp_count_impl<L, V>::type;

/*************************** COUNT_IF metafunction. ***************************/

namespace detail {

template<class L, template<class...> class P>
struct mp_count_if_impl;

template<template<class...> class L, class... T, template<class...> class P>
struct mp_count_if_impl<L<T...>, P> {
    using type = mp_plus<mp_bool<P<T>::value>...>;
};

} // detail

template<class L, template<class...> class P>
using mp_count_if = typename detail::mp_count_if_impl<L, P>::type;

/************************* CONTAINS_IF metafunction. **************************/

template<class L, template<class...> class P>
using mp_contains_if = mp_bool<mp_count_if<L, P>::value != 0>;

/*************************** ALL metafunction. ***************************/

namespace detail {

template<class L, class V>
struct mp_all_impl;

template<class... T>
struct mp_all_impl2 : mp_false {};

template<class V>
struct mp_all_impl2<V> : mp_true {};

template<class V>
struct mp_all_impl2<V, V> : mp_true {};

template<class V, class... T>
struct mp_all_impl2<V, V, T...> : mp_all_impl2<V, T...> {};

template<template<class...> class L, class... T, class V>
struct mp_all_impl<L<T...>, V> : mp_all_impl2<V, T...> {};

} // detail

template<class L, class V>
using mp_all = typename detail::mp_all_impl<L, V>::type;

template<class L, class V>
using mp_not_all = mp_bool<!mp_all<L, V>::value>;

template <class L, class V>
constexpr bool mp_isall() {
    return mp_all<L, V>::value;
}

/*************************** ANY metafunction. ***************************/

namespace detail {

template<class L, class V>
struct mp_any_impl;

template<class... T>
struct mp_any_impl2 : mp_false {};

template<class V, class... T>
struct mp_any_impl2<V, V, T...> : mp_true {};

template<class V, class T1, class... T>
struct mp_any_impl2<V, T1, T...> : mp_any_impl2<V, T...> {};

template<template<class...> class L, class... T, class V>
struct mp_any_impl<L<T...>, V> : mp_any_impl2<V, T...> {};

} // detail

template<class L, class V>
using mp_any = typename detail::mp_any_impl<L, V>::type;

template<class L, class V>
using mp_not_any = mp_bool<!mp_any<L, V>::value>;

template <class L, class V>
constexpr bool mp_isany() {
    return mp_any<L, V>::value;
}

/*************************** CONTAINS metafunction. ***************************/

namespace detail {

template<class L, class T>
struct mp_contains_impl;

template<template<class...> class L, class... T, template<class...> class L2, class... V>
struct mp_contains_impl<L<T...>, L2<V...>> {
    using type = mp_all<mp_list<mp_any<L<T...>, V>...>, mp_true>;
};

} // detail

template<class L, class V>
using mp_contains = typename detail::mp_contains_impl<L, V>::type;

template<class L, class V>
using mp_not_contains = mp_bool<!mp_contains<L, V>::value>;

template <class L, class V>
constexpr bool mp_iscontains() {
    return mp_contains<L, V>::value;
}

/*************************** SAME metafunction. ***************************/

template <class V, class... T>
constexpr bool mp_issame() {
    return std::is_same<V, T...>::value;
}

/*************************** HASFUNCTION metafunction. ***************************/

template<class...> struct voider {
    using type = void;
};

template<class... T>
using void_t = typename voider<T...>::type;

// template<class...>
// using void_t = void;

#define mp_hasfn_with_signature_declare(FNNAME)                         \
    template<typename, typename T>                                      \
    struct mp_hasfn_with_signature_##FNNAME##_t {                       \
        static_assert(                                                  \
            std::integral_constant<T, false>::value,                    \
            "Second template parameter needs to be of function type."); \
    };                                                                  \
                                                                        \
    template<typename C, typename Ret, typename... Args>                \
    struct mp_hasfn_with_signature_##FNNAME##_t<C, Ret(Args...)> {      \
    private:                                                            \
    template<typename T>                                                \
    static constexpr auto check(T*)                                     \
        -> typename                                                     \
        std::is_same<decltype(std::declval<T>().FNNAME(std::declval<Args>()... )), \
                     Ret                                                \
                     >::type;                                           \
                                                                        \
    template<typename>                                                  \
    static constexpr std::false_type check(...);                        \
                                                                        \
    typedef decltype(check<C>(0)) type;                                 \
                                                                        \
    public:                                                             \
    static constexpr bool value = type::value;                          \
    };                                                                  \
    template<class... T>                                                \
    constexpr bool mp_hasfn_with_signature_##FNNAME() {                 \
        return mp_hasfn_with_signature_##FNNAME##_t<T...>::value;       \
    }

#define mp_hasfn_with_signature(CLASS, FNNAME, FN)  \
    mp_hasfn_with_signature_##FNNAME<CLASS, FN>()

#define mp_hasfn_with_params_declare(FNNAME)                            \
    template<typename C, typename... Args>                              \
    struct mp_hasfn_with_params_##FNNAME##_t {                          \
    private:                                                            \
    template<class T, class = void>                                     \
    struct mp_hasfunction_ : mp_false {};                               \
                                                                        \
    template<class T>                                                   \
    struct mp_hasfunction_<T,                                           \
                           void_t<                                      \
                              decltype(                                 \
                                  std::declval<T&>().FNNAME(std::declval<Args>()... ))>> \
        : mp_true {};                                                   \
                                                                        \
    public:                                                             \
    using type = typename mp_hasfunction_<C>::type;                     \
    static constexpr bool value = type::value;                          \
    };                                                                  \
    template<class... T>                                                \
    constexpr bool mp_hasfn_with_params_##FNNAME() {                    \
        return mp_hasfn_with_params_##FNNAME##_t<T...>::value;          \
    }

#define mp_hasfn_with_params(CLASS, FNNAME, FN)  \
    mp_hasfn_with_params_##FNNAME<CLASS, FN>()

} /* namespace mp11 */

#endif /* MP11_HPP */
