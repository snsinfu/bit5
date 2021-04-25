#include <cstddef>
#include <type_traits>
#include <utility>

namespace meta
{
    struct any
    {
        template<typename T>
        operator T() const;
    };

    template<typename Record, typename = std::index_sequence<>, typename = void>
    struct record_size
    {
        static constexpr std::size_t value = 0;
    };

    template<typename Record, std::size_t... Seq>
    struct record_size<
        Record,
        std::index_sequence<Seq...>,
        std::void_t<decltype(Record{any{}, (Seq, any{})...})>
    >
    {
        static constexpr std::size_t value = 1 + record_size<
                Record, std::index_sequence<Seq..., sizeof...(Seq)>
            >::value;
    };

    template<typename Record>
    inline constexpr std::size_t record_size_v = record_size<Record>::value;
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

    std::cout << meta::record_size_v<record> << '\n';
}
