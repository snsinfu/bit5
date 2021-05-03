#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>


namespace tr
{
    struct hook {};

    class assert_error : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    template<typename T>
    class pretty
    {
    public:
        explicit pretty(T const& value)
            : _value{value}
        {
        }

        T const& value() const
        {
            return _value;
        }

    private:
        T const& _value;
    };

    template<
        typename T,
        typename = decltype(std::declval<std::ostream&>() << std::declval<T const&>())
    >
    std::ostream& operator<<(std::ostream& os, tr::pretty<T> const& pr)
    {
        return os << pr.value();
    }

    inline
    std::ostream& operator<<(std::ostream& os, tr::pretty<char> const& pr)
    {
        return os << '\'' << pr.value() << '\'';
    }

    template<typename... T>
    std::ostream& operator<<(std::ostream& os, tr::pretty<T...> const&)
    {
        return os << "{?}";
    }
 
    class check
    {
    public:
        template<typename T>
        void check_trace(tr::hook, std::string_view expr, T const& value)
        {
            _trace
                << ' '
                << expr
                << " = "
                << tr::pretty{value}
                << '\n';
        }

        void check_assert(tr::hook, std::string_view expr, bool pred)
        {
            if (!pred) {
                std::string message{expr};
                message += "\nTrace:\n";
                message += _trace.str();
                throw tr::assert_error{message};
            }
            _trace.str("");
        }

    private:
        std::ostringstream _trace;
    };

    inline
    tr::check& global()
    {
        static tr::check instance;
        return instance;
    }

    template<typename T>
    void check_trace(tr::hook, std::string_view expr, T const& value)
    {
        tr::global().check_trace(tr::hook{}, expr, value);
    }

    inline
    void check_assert(tr::hook, std::string_view expr, bool pred)
    {
        tr::global().check_assert(tr::hook{}, expr, pred);
    }
}

#define TRACE(X) check_trace(tr::hook{}, #X, (X))
#define ASSERT(X) check_assert(tr::hook{}, #X, (X))

// ---------------------------------------------------------------------------

#include <exception>
#include <iostream>
#include <string_view>

std::string_view trim_until(std::string_view str, char delim)
{
    std::string_view::size_type pos = 0;

    for (; pos < str.size(); pos++) {
        TRACE(pos);
        TRACE(str[pos]);

        if (str[pos] == delim) {
            break;
        }
    }
    pos++;

    ASSERT(pos <= str.size());

    return str.substr(pos);
}

int main()
{
    try {
        trim_until("a|b|c", '|');
        trim_until("abc", '|');
    } catch (std::exception const& err) {
        std::cerr << "error: " << err.what() << '\n';
    }
}
