#include <stddef.h>
#include <stdio.h>


int main(void)
{
    enum { num_elements = 5 };

    double values[num_elements];

    for (size_t i = 0; i < num_elements; i++) {
        values[i] = (double) (1 + i) / 10.0;
    }

    // Determine the size (number of nodes) of the minimum binary tree that
    // can store the elements in its leaves.
    size_t leaf_count = 1;

    for (;;) {
        if (leaf_count >= num_elements) {
            break;
        }
        leaf_count *= 2;
    }

    size_t const tree_size = leaf_count * 2 - 1;

    // Construct a binary tree from five leaf elements:
    //
    //               AA
    //              /  \
    //             /    \
    //            /      \
    //           /        \
    //          /          \
    //        aa            bb
    //       /  \          /  \
    //      /    \        /    \
    //     a      b      c      *
    //    / \    / \    / \    / \
    //   0   1  2   3  4   *  *   *
    //
    // Layout the tree in array form:
    //
    //   [ AA  aa  bb  a  b  c  *  0  1  2  3  4  *  *  * ]
    //
    // The number of array elements would be 2**(1+ceil(log2(n)))-1 if n leaf
    // elements are given. 4n is always sufficient.

    double sumtree[num_elements * 4] = { 0, };

    // Fill leaves.
    for (size_t i = 0; i < num_elements; i++) {
        sumtree[leaf_count + i] = values[i];
    }

    // Construct sum tree.
    for (size_t layer = 1; ; layer++) {
        size_t const layer_size = leaf_count >> layer;
        size_t const start = layer_size - 1;
        size_t const end = start + layer_size;
        if (layer_size == 0) {
            break;
        }

        for (size_t i = start; i < end; i++) {
            size_t const lchild = 2 * (i + 1) - 1;
            size_t const rchild = 2 * (i + 1);
            sumtree[i] = sumtree[lchild] + sumtree[rchild];
        }
    }

    // Print array representation of the tree.
    for (size_t i = 0; i < tree_size; i++) {
        printf("%2zu  %g\n", i, sumtree[i]);
    }
}
