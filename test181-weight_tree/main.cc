#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>
#include <vector>


namespace
{
    using std::size_t;

    class weight_tree
    {
    public:

        /*
         * Constructs an empty weight tree.
         */
        weight_tree() = default;


        /*
         * Constructs a weight tree from given weights.
         *
         * Params:
         *   weights = Array of weight values of elements.
         *
         * Time complexity:
         *   O(N) where N is the number of elements (= `weights.size()`).
         */
        explicit weight_tree(std::vector<double> const& weights)
        {
            size_t leaves = 1;

            for (;;) {
                if (leaves >= weights.size()) {
                    break;
                }
                leaves *= 2;
            }

            _sumtree.resize(leaves * 2 - 1);
            _leaves = leaves;
            _elements = weights.size();
            assert(_leaves >= _elements);

            for (size_t i = 0; i < _elements; i++) {
                _sumtree[_leaves + i - 1] = weights[i];
            }

            for (size_t layer = 1; ; layer++) {
                auto const layer_size = leaves >> layer;
                if (layer_size == 0) {
                    break;
                }

                auto const start = layer_size - 1;
                auto const end = start + layer_size;

                assert(end < _leaves);
                assert(start < end);

                for (size_t node = start; node < end; node++) {
                    auto const lchild = 2 * node + 1;
                    auto const rchild = 2 * node + 2;
                    _sumtree[node] = _sumtree[lchild] + _sumtree[rchild];
                }
            }
        }


        /*
         * Updates the weight of i-th element, propagating the change to
         * relevant nodes.
         *
         * Behavior is undefined if `i` is out of range or `weight` is negative
         * or not finite. It is also undefined that the sum of weights overflow
         * due to the update.
         *
         * Params:
         *   i      = Index of the element to update weight.
         *   weight = New weight value.
         *
         * Time complexity:
         *   O(log N) where N is the number of elements.
         */
        void update(size_t i, double weight)
        {
            assert(i < _elements);
            assert(weight >= 0);

            size_t node = _leaves + i - 1;
            _sumtree[node] = weight;

            do {
                node = (node - 1) / 2;
                auto const lchild = 2 * node + 1;
                auto const rchild = 2 * node + 2;
                _sumtree[node] = _sumtree[lchild] + _sumtree[rchild];
            } while (node > 0);
        }


        /*
         * Gets the number of elements.
         */
        size_t size() const noexcept
        {
            return _elements;
        }


        /*
         * Gets the weight of an element.
         */
        double operator[](size_t i) const
        {
            return _sumtree[_leaves + i - 1];
        }


        /*
         * Gets the total weight.
         *
         * Returns:
         *   The sum of the weight values of all the elements stored.
         *
         * Time complexity: O(1).
         */
        double sum() const noexcept
        {
            return _sumtree[0];
        }


        /*
         * Finds the element whose cumulative weight range covers given probe
         * value.
         *
         * Returns:
         *   The index of the found element.
         *
         * Time complexity:
         *   O(log N) where N is the number of elements.
         */
        size_t find(double probe) const
        {
            size_t node = 0;

            for (;;) {
                auto const lchild = 2 * node + 1;
                auto const rchild = 2 * node + 2;

                if (lchild >= _sumtree.size()) {
                    break;
                }

                if (probe < _sumtree[lchild]) {
                    node = lchild;
                } else {
                    probe -= _sumtree[lchild];
                    node = rchild;
                }
            }

            assert(node >= _leaves - 1);

            return node - (_leaves - 1);
        }

    private:
        std::vector<double> _sumtree;
        size_t              _leaves = 0;
        size_t              _elements = 0;
    };
}


int main()
{
    weight_tree weights(std::vector<double>(100, 1.0));

    std::vector<long> histogram(weights.size());
    long const samples = 100000;
    std::mt19937_64 random;
    std::uniform_real_distribution<double> uniform;

    for (long i = 0; i < samples; i++) {
        auto const probe = weights.sum() * uniform(random);
        auto const choice = weights.find(probe);
        histogram[choice]++;

        // Lucky gets luckier. Note: This exponential growth may overflow the
        // weight (and causes UB) if this loop is too long.
        weights.update(choice, weights[choice] * 1.005);
    }

    for (std::size_t i = 0; i < histogram.size(); i++) {
        auto const percent = std::size_t(histogram[i] * 100 / samples);

        std::cout
            << i
            << ":\t"
            << std::string(percent + 1, '|')
            << ' '
            << percent
            << "%\n";
    }
}
