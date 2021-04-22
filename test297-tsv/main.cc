#include <charconv>
#include <cstddef>
#include <istream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>


namespace tsv
{
    template<typename Record>
    std::vector<Record> load(std::istream& src);

    template<typename Record>
    std::vector<Record> load(std::istream&& src);

    class error : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class parse_error : public tsv::error
    {
    public:
        using tsv::error::error;
    };

    namespace detail
    {
        struct any
        {
            template<typename T>
            operator T() const;
        };

        template<std::size_t N>
        struct rank : rank<N - 1> {};

        template<>
        struct rank<0> {};

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
        using record_size = decltype(probe<R>(new rank<9>, nullptr));
    }

    namespace detail
    {
        template<typename... Ts>
        struct type_list {};

        template<typename... Ts>
        type_list<Ts...> make_type_list(Ts const&...);

#define X(N, ...)                                                       \
    template<typename R>                                                \
    auto splat(R const& rec, std::integral_constant<std::size_t, N>)    \
    {                                                                   \
        auto [__VA_ARGS__] = rec;                                       \
        return make_type_list(__VA_ARGS__);                             \
    }

        X(1, a1)
        X(2, a1, a2)
        X(3, a1, a2, a3)
        X(4, a1, a2, a3, a4)
        X(5, a1, a2, a3, a4, a5)
        X(6, a1, a2, a3, a4, a5, a6)
        X(7, a1, a2, a3, a4, a5, a6, a7)
        X(8, a1, a2, a3, a4, a5, a6, a7, a8)
        X(9, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#undef X

        template<typename R>
        using field_type_list = decltype(splat(std::declval<R>(), record_size<R>{}));
    }

    namespace detail
    {
        inline std::string_view split_consume(std::string_view& text, char delim)
        {
            auto const pos = text.find(delim);
            if (pos != std::string_view::npos) {
                auto const token = text.substr(0, pos);
                text = text.substr(pos + 1);
                return token;
            } else {
                auto const token = text;
                text = text.substr(text.size());
                return token;
            }
        }

        template<typename T>
        T parse(std::string_view text)
        {
            auto const begin = text.data();
            auto const end = text.data() + text.size();
            T value;

            auto const [remain, err] = std::from_chars(begin, end, value);
            if (err != std::errc{}) {
                throw parse_error{"conversion"};
            }
            if (remain != end) {
                throw parse_error{"excess characters"};
            }

            return value;
        }

        template<>
        inline std::string parse<std::string>(std::string_view text)
        {
            return std::string{text};
        }

        template<typename Record, typename... Ts>
        Record parse_record(std::string_view text, type_list<Ts...>)
        {
            return {parse<Ts>(split_consume(text, '\t'))...};
        }
    }

    template<typename Record>
    std::vector<Record> load(std::istream& src)
    {
        std::vector<Record> records;
        detail::field_type_list<Record> field_types;
        for (std::string line; std::getline(src, line); ) {
            records.push_back(detail::parse_record<Record>(line, field_types));
        }
        return records;
    }

    template<typename Record>
    std::vector<Record> load(std::istream&& src)
    {
        return tsv::load<Record>(src);
    }
}


#include <fstream>
#include <iostream>

int main()
{
    struct record
    {
        std::string source;
        std::string destination;
        int kind;
    };

    auto const recs = tsv::load<record>(std::ifstream{"sample.tsv"});
    for (auto const& rec : recs) {
        std::cout << rec.kind << '\n';
    }
}
