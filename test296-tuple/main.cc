#include <cstddef>
#include <tuple>
#include <type_traits>


namespace detail
{
    struct any
    {
        template<typename T>
        operator T() const;
    };

    template<std::size_t N>
    struct rank : rank<N - 1>
    {
    };

    template<>
    struct rank<0>
    {
    };

#define X(N, ...)        \
    template<typename R> \
    std::integral_constant<std::size_t, N> probe(rank<N>*, decltype(R{__VA_ARGS__})*)

    X(1, any{});
    X(2, any{}, any{});
    X(3, any{}, any{}, any{});
    X(4, any{}, any{}, any{}, any{});
    X(5, any{}, any{}, any{}, any{}, any{});
    X(6, any{}, any{}, any{}, any{}, any{}, any{});
    X(7, any{}, any{}, any{}, any{}, any{}, any{}, any{});
    X(8, any{}, any{}, any{}, any{}, any{}, any{}, any{}, any{});
    X(9, any{}, any{}, any{}, any{}, any{}, any{}, any{}, any{}, any{});
#undef X

    template<typename R>
    using probe_t = decltype(probe<R>(new rank<9>, nullptr));
}

namespace detail
{
#define X(N, ...)                                                       \
    template<typename R>                                                \
    auto splat(R const& rec, std::integral_constant<std::size_t, N>)    \
    {                                                                   \
        auto [__VA_ARGS__] = rec;                                       \
        return std::make_tuple(__VA_ARGS__);                            \
    }

    X(1, a1);
    X(2, a1, a2);
    X(3, a1, a2, a3);
    X(4, a1, a2, a3, a4);
    X(5, a1, a2, a3, a4, a5);
    X(6, a1, a2, a3, a4, a5, a6);
    X(7, a1, a2, a3, a4, a5, a6, a7);
    X(8, a1, a2, a3, a4, a5, a6, a7, a8);
    X(9, a1, a2, a3, a4, a5, a6, a7, a8, a9);
#undef X

    template<typename R>
    auto splat(R const& rec)
    {
        return splat(rec, probe_t<R>{});
    }
}


#include <iostream>

int main()
{
    struct record
    {
        std::string id;
        int kind;
        double value;
    };

    record rec = {"f41da36", 1, 2.34};
    auto tup = detail::splat(rec);

    std::cout << std::get<0>(tup) << '\n';
    std::cout << std::get<1>(tup) << '\n';
    std::cout << std::get<2>(tup) << '\n';
}
