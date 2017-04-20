# mp11
Template Metaprogramming Library For Cpp11

Usefull mpl for c++11+ which can serve as small set of metafunction for working with sets of types lists.
Its implemented some basic meta functions: isall, isany, true, false, list, issame, length, size, transform, rename, contains, void_t.
Also it has convenient method for checking class method existsing at compile time.

More examples in test.cpp.

```c++
static_assert(
    mp_isany<mp_list<int, long, char>, int>(), "");
static_assert(
    mp_isall<mp_list<int, int, int>, int>(), "");
static_assert(
    mp_iscontains<mp_list<int, char>, mp_list<char, int>>(), "");
static_assert(
    mp_count<mp_list<int, int, long, char>, int>::value == 2, "");
static_assert(
    mp_issame<mp_push_back<mp_list<int, char>, long>, mp_list<int, char, long>>(), "");
static_assert(
    mp_issame<mp_pop_back<mp_list<int, char, long>>, mp_list<int, char>>(), "");
static_assert(
    mp_issame<mp_push_front<mp_list<int, char>, long>, mp_list<long, int, char>>,"");
static_assert(
    mp_issame<mp_pop_front<mp_list<int, char, long>>, mp_list<char, long>>(), "");

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

mp_hasfn_with_signature_declare(fn);
static_assert(mp_hasfn_with_signature(Foo, fn, void(void)), "");

mp_hasfn_with_params_declare(barwitharg);
static_assert(mp_hasfn_with_params(Bar, barwitharg, int), "");
```
