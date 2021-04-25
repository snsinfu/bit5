#include <cstddef>
#include <utility>

namespace meta
{
    template<typename... Types>
    struct type_list
    {
        static constexpr std::size_t size = sizeof...(Types);
    };

    template<typename... Types>
    type_list<Types...> make_type_list(Types const&...)
    {
        return {};
    }

    template<std::size_t N>
    using rank = std::integral_constant<std::size_t, N>;

#define X(N, ...)                               \
    template<typename Record>                   \
    auto probe(Record const& record, rank<N>)   \
    {                                           \
        auto [__VA_ARGS__] = record;            \
        return make_type_list(__VA_ARGS__);     \
    }

    // TODO: Auto-generate the following list.
    X(1, a1)
    X(2, a1, a2)
    X(3, a1, a2, a3)
    X(4, a1, a2, a3, a4)

#undef X

    template<typename Record, std::size_t Size>
    using probe_t = decltype(probe(std::declval<Record>(), rank<Size>{}));
}

#include <iostream>

int main()
{
    struct record
    {
        int source;
        int destination;
        char label;
        double weight;
    };

    // Here we hard-code the number of fields "4" but the number can be deduced
    // with metaprogramming. See test299.
    using types = meta::probe_t<record, 4>;

    std::cout << types::size << '\n';
}
