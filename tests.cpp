#include "mp11.hpp"

#include <vector>
#include <set>
#include <iostream>
#include <tuple>

using namespace mp11;

void fn(int) {}

static_assert(
    mp_issame<mp_rename<std::vector<int>, std::set>,
    std::set<int, std::allocator<int>, std::allocator<int>>>(), "");

static_assert(
    mp_length<int, long, void, void>() == 4, "");

static_assert(
    mp_size<mp_list<int, long, long, char>>() == 4, "");

static_assert(
    mp_issame<mp_true, mp_empty<mp_list<>>>(), "");

static_assert(
    mp_issame<mp_false, mp_empty<mp_list<int>>>(), "");

static_assert(
    mp_issame<mp_clear<mp_list<>>, mp_list<>>(), "");

static_assert(
    mp_issame<mp_clear<mp_list<int, char, long>>, mp_list<>>(), "");

static_assert(
    mp_issame<mp_front<mp_list<int, char, long>>, int>(), "");

static_assert(
    mp_issame<mp_back<mp_list<int, char, long>>, long>(), "");

static_assert(
    mp_issame<mp_pop_front<mp_list<int, char, long>>, mp_list<char, long>>(), "");

static_assert(
    mp_issame<mp_pop_back<mp_list<int, char, long>>, mp_list<int, char>>(), "");

static_assert(
    mp_issame<mp_push_front<mp_list<int, char>, long>, mp_list<long, int, char>>,"");

static_assert(
    mp_issame<mp_push_back<mp_list<int, char>, long>, mp_list<int, char, long>>(), "");

static_assert(
    mp_plus<std::is_same<int, int>, std::is_same<int, int>>::value == 2, "");

static_assert(
    mp_count<mp_list<int, int, long, char>, int>::value == 2, "");

static_assert(
    mp_iscontains<mp_list<>, mp_list<>>(), "");

static_assert(
    mp_iscontains<mp_list<int>, mp_list<>>(), "");

static_assert(
    mp_iscontains<mp_list<int, char>, mp_list<char>>(), "");

static_assert(
    mp_iscontains<mp_list<int, char>, mp_list<char, int>>(), "");

static_assert(
    !mp_iscontains<mp_list<int, char>, mp_list<long>>(), "");

static_assert(
    mp_issame<mp_true, mp_contains<mp_list<int, char>, mp_list<int>>>(), "");

static_assert(
    mp_issame<mp_true, mp_not_contains<mp_list<int, char>, mp_list<long>>>(), "");

static_assert(
    !mp_isall<mp_list<int, char>, long>(), "");

static_assert(
    !mp_isall<mp_list<int, int, char>, int>(), "");

static_assert(
    mp_isall<mp_list<int, int, int>, int>(), "");

static_assert(
    mp_isany<mp_list<int>, int>(), "");

static_assert(
    mp_isany<mp_list<int, long, char>, int>(), "");

static_assert(
    !mp_isany<mp_list<int, long>, char>(), "");

template<class T> struct mp_is_int_impl;

template<>
struct mp_is_int_impl<int> {
    using type = mp_true;
};

template<class T>
struct mp_is_int_impl {
    using type = mp_false;
};

template<class T>
using mp_is_int = typename mp_is_int_impl<T>::type;

static_assert(
    mp_issame<mp_true, mp_contains_if<mp_list<int>, mp_is_int>>(), "");

static_assert(
    mp_issame<mp_false, mp_contains_if<mp_list<char>, mp_is_int>>(), "");

using input = std::tuple<int, char, long>;
using expected = std::tuple<const int*, const char*, const long*>;
using result = mp_transform<mp_add_pointer, mp_transform<mp_add_const, input>>;
static_assert(mp_issame<expected, result>(), "");

using input2 = std::pair<mp_list<int, char, long>, mp_list<int, char, long>>;
using expected2 = mp_list<std::pair<int, int>, std::pair<char, char>, std::pair<long, long>>;
using result2 = mp_transform<std::pair, input2::first_type, input2::second_type>;
static_assert(mp_issame<expected2, result2>(), "");

class Foo {
public:
    int foo() { return {}; }
    void fn() {}
    int doo(int) { return {};};
};

class Bar {
public:
    Bar() = delete;
    explicit Bar(const int) {}
    void barfn() {}
    void barwithoutarg() {}
    void barwitharg(int) {}
};


mp_hasfn_with_signature_declare(doo);
mp_hasfn_with_signature_declare(fn);

static_assert(mp_hasfn_with_signature_doo<Foo, int(int)>(), "");
static_assert(mp_hasfn_with_signature_fn<Foo, void(void)>(), "");

static_assert(mp_hasfn_with_signature(Foo, fn, void(void)), "");

mp_hasfn_with_params_declare(barwitharg);
mp_hasfn_with_params_declare(barwithoutarg);

static_assert(mp_hasfn_with_params_barwithoutarg<Bar>(), "");
static_assert(mp_hasfn_with_params_barwitharg<Bar, int>(), "");

static_assert(mp_hasfn_with_params(Bar, barwitharg, int), "");

int main()
{
    //fn(decltype(std::declval<Foo>().fn()));
    return 0;
}
