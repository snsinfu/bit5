#include <iostream>
#include <utility>


namespace
{
    struct state_type
    {
        int cookie = 0;
    };

    template<
        typename F,
        typename = decltype(std::declval<F>()(int{}, int{}))
    >
    void detect_binary();

    template<
        typename F,
        typename = decltype(
            std::declval<F>()(std::declval<state_type&>(), int{}, int{})
        )
    >
    void detect_ternary();

    template<typename F, typename = void>
    struct standardized_functor;

    template<typename F>
    struct standardized_functor<F, decltype(detect_binary<F>())>
    {
        F impl;

        auto operator()(state_type&, int i, int j)
        {
            return impl(i, j);
        }
    };

    template<typename F>
    struct standardized_functor<F, decltype(detect_ternary<F>())>
    {
        F impl;

        auto operator()(state_type& state, int i, int j)
        {
            return impl(state, i, j);
        }
    };

    template<typename F>
    standardized_functor<F> standardize(F fun)
    {
        return standardized_functor<F>{fun};
    }
}


int main()
{
    auto f = [](int i, int j) {
        std::cout << "f: i=" << i << ", j=" << j << '\n';
    };

    auto g = [](state_type& state, int i, int j) {
        std::cout << "g: cookie=" << state.cookie << ", i=" << i << ", j=" << j << '\n';
    };

    state_type state {
        .cookie = 1234,
    };

    standardize(f)(state, 1, 2);
    standardize(g)(state, 3, 4);

    // This will correctly fail.
    // standardize([](int k) {})(state, 3, 4);
}
