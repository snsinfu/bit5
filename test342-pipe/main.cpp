#include <iterator>
#include <type_traits>
#include <utility>


inline constexpr struct with_index_hook {} with_index;

template<typename R>
class with_index_adaptor
{
    using base_iterator = decltype(std::begin(std::declval<R>()));

public:
    class sentinel
    {
    public:
        explicit sentinel(base_iterator const& base)
            : base{base}
        {
        }

        base_iterator base;
    };

    class iterator
    {
    public:
        using index_type = std::size_t;
        using value_type = std::pair<
            index_type, typename std::iterator_traits<base_iterator>::reference
        >;
        using reference = value_type;
        using pointer = void;
        using iterator_category = std::input_iterator_tag;

        explicit iterator(base_iterator const& base)
            : base{base}
        {
        }

        iterator& operator++()
        {
            ++base;
            ++index;
            return *this;
        }

        iterator operator++(int)
        {
            auto old = this;
            ++*this;
            return old;
        }

        reference operator*() const
        {
            return {index, *base};
        }

        bool operator==(iterator const& other) const
        {
            return base == other.base;
        }

        bool operator!=(iterator const& other) const
        {
            return !(*this == other);
        }

        bool operator==(sentinel const& other) const
        {
            return base == other.base;
        }

        bool operator!=(sentinel const& other) const
        {
            return !(*this == other);
        }

    private:
        base_iterator base;
        index_type index = 0;
    };

    explicit with_index_adaptor(R& source)
        : source{source}
    {
    }

    iterator begin() const
    {
        return iterator{std::begin(source)};
    }

    sentinel end() const
    {
        return sentinel{std::end(source)};
    }

private:
    R& source;
};

template<typename R>
with_index_adaptor<std::remove_reference_t<R>> operator|(R&& source, with_index_hook)
{
    return with_index_adaptor<std::remove_reference_t<R>>{source};
}


// -----------------------------------------------------------------------------

#include <iostream>
#include <vector>


int main()
{
    std::vector<double> numbers = {
        1.1, 2.3, 5.8, 11.19, 30.49, 79.128, 207.335, 542.877
    };

    for (auto const& [i, num] : numbers | with_index) {
        std::cout << i << '\t' << num << '\n';
    }
}
