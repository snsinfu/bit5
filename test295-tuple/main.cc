#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

// This does not work. SFINAE cannot tolerate semantic errors in the body of
// a function used in the tested expression.

namespace tsv
{
    namespace detail
    {
#define X(N, ...)                               \
    template<typename R>                        \
    auto splat##N(R const& r)                   \
    {                                           \
        auto [__VA_ARGS__] = r;                 \
        return std::make_tuple(__VA_ARGS__);    \
    }
        X(1, a1)
        X(2, a1, a2)
        X(3, a1, a2, a3)
        X(4, a1, a2, a3, a4)
        X(5, a1, a2, a3, a4, a5)
        X(6, a1, a2, a3, a4, a5, a6)
        X(7, a1, a2, a3, a4, a5, a6, a7)
#undef X

        template<typename R> auto probe_splat(R const& r, decltype(splat1(r))* p) -> decltype(*p);
        template<typename R> auto probe_splat(R const& r, decltype(splat2(r))* p) -> decltype(*p);
        template<typename R> auto probe_splat(R const& r, decltype(splat3(r))* p) -> decltype(*p);
        template<typename R> auto probe_splat(R const& r, decltype(splat4(r))* p) -> decltype(*p);
        template<typename R> auto probe_splat(R const& r, decltype(splat5(r))* p) -> decltype(*p);
        template<typename R> auto probe_splat(R const& r, decltype(splat6(r))* p) -> decltype(*p);
        template<typename R> auto probe_splat(R const& r, decltype(splat7(r))* p) -> decltype(*p);

        template<typename R>
        using splat_t = decltype(probe_splat(std::declval<R>(), nullptr));
    }

    template<typename Record>
    Record parse(std::string const& line)
    {
        Record rec;

        detail::splat_t<Record> tup;

        return rec;
    }
}


int main()
{
    struct record
    {
        std::string id;
        int kind;
        double value;
    };
    tsv::parse<record>("84cd70af4b50ab93\t95\t3.21");

}
