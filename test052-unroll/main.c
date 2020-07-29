// Inverting rolling sum (windowed sum).
//
// See:
// https://codegolf.stackexchange.com/questions/207283/recover-boolean-cells-from-windowed-sums
//
// There are bruteforce answers. Is there an efficient algorithm?
//

#include <stddef.h>
#include <stdio.h>


// For N = 6:
//
//   r(0) = x(0) + x(1)
//   r(1) = x(0) + x(1) + x(2)
//   r(2) =        x(1) + x(2) + x(3)
//   r(3) =               x(2) + x(3) + x(4)
//   r(4) =                      x(3) + x(4) + x(5)
//   r(5) =                             x(4) + x(5)
//
// Firstly, I have
//
//   x(2) = r(1) - r(0) .
//
// Can I have more? Cumsum?
//
//   cumsum r(0) =   x(0) +   x(1)
//   cumsum r(1) = 2 x(0) + 2 x(1) +   x(2)
//   cumsum r(2) = 2 x(0) + 3 x(1) + 2 x(2) +   x(3)
//   cumsum r(3) = 2 x(0) + 3 x(1) + 3 x(2) + 2 x(3) +   x(4)
//   cumsum r(4) = 2 x(0) + 3 x(1) + 3 x(2) + 3 x(3) + 2 x(4) +   x(5)
//   cumsum r(5) = 2 x(0) + 3 x(1) + 3 x(2) + 3 x(3) + 3 x(4) + 2 x(5)
//
// We get this:
//
//   x(2) = cumsum r(1) - 2 cumsum r(0)
//
// Not so much I gained it seems.
//




int main()
{
    enum {
        size = 9
    };

    int const truth[size] = {
        0, 1, 0, 0, 1, 1, 0, 1, 0
    };

    int const rolling_sums[size] = {
        1, 1, 1, 1, 2, 2, 2, 1, 1
    };

    int rolling_cumsums[size];

    for (size_t i = 0; i < size; i++) {
        rolling_cumsums[i] = 0;
        for (size_t j = 0; j <= i; j++) {
            rolling_cumsums[i] += rolling_sums[j];
        }
    }

    int x2 = rolling_sums[1] - rolling_sums[0];
    printf("%d\n", x2);
}
