#include <stddef.h>
#include <stdio.h>


int main(void)
{
    enum { num_elements = 5 };

    double values[num_elements];

    for (size_t i = 0; i < num_elements; i++) {
        values[i] = (double) i;
    }

    // For values = [x0, x1, x2, x3, x4], the sumtree would look like:
    //
    //   [0] [1] [2] [3] [4] : [5]   [6]   [7] : [8]   [9] : [10]
    //   s0  x1  s2  s3  s4  : s5    s6    s7  : s8    s9  : s10
    //   ||  ||  ||  ||  ||  : ||    ||    ||  : ||    ||  : ||
    //   x0  x1  x2  x3  x4  : s0+s1 s2+s3 s4  : s5+s6 s7  : s8+s9
    //

    double sumtree[num_elements * 2 + 1];

    for (size_t i = 0; i < num_elements; i++) {
        sumtree[i] = values[i];
    }

    size_t prev_offset = 0;
    size_t offset = num_elements;
    size_t layer = (num_elements + 1) / 2;

    for (;;) {
        for (size_t i = 0; i < layer; i++) {
            size_t child = prev_offset + i * 2;
            if (child + 1 < offset) {
                sumtree[offset + i] = sumtree[child] + sumtree[child + 1];
            } else {
                sumtree[offset + i] = sumtree[child];
            }
        }

        prev_offset = offset;
        offset += layer;

        if (layer == 1) {
            break;
        }
        layer = (layer + 1) / 2;
    }

    for (size_t i = 0; i < offset; i++) {
        printf("%g\n", sumtree[i]);
    }

    // So, I arranged numbers in a non-heap-like layout. But how can I search
    // in the tree? Scanning from the tail does not work easily because there
    // can be odd number of internal nodes...
}
