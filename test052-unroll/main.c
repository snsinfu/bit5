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

// With
//
//   x(2) = r(1) - r(0)
//
// I can create the rolling sum of the sub-seqeuence x(3:):
//
//   R(0) = r(3) - x(2) = x(3) + x(4)
//   R(1) = r(4)        = x(3) + x(4) + x(5)
//   R(2) = r(5)        =        x(4) + x(5) + x(6)
//
// Now I can compute another truth:
//
//   x(5) = R(1) - R(0)
//        = r(4) - r(3) + x(2) .
//
// Similarly I get x(2), x(5), x(8), x(11), ...

// For i = 0 and 1 the input gives
//
//   r(0) = x(0) + x(1)
//   r(1) = x(0) + x(1) + x(2)
//   r(2) =        x(1) + x(2) + x(3)
//
// and I can use x(2) to get
//
//   r(0)        = x(0) + x(1)
//   r(1) - x(2) = x(0) + x(1)
//   r(2) - x(2) =        x(1) + x(3) .
//
// This system of equations is solvable once I get x(3). However, to get x(3):
//
//   r(3) - x(2) = x(3) + x(4)
//   r(4) - x(5) = x(3) + x(4)
//   r(5) - x(5) =        x(4) + x(6)
//
// I need x(6). Similarly, to get x(6) I need x(9), which requires x(12), which
// requires x(15), and so on. So I just need to brute force x(3p) where p is the
// maximum integer that satisfies 3p < N.

// Algorithm. Firstly, solve for x(3k-1) where k = 1,2,3,..., Then, bruteforce
// on x(3p) to get:
//
//   x(3p-2) = r(3p-1) - x(3p-1) - x(3p)
//   x(3p-3) = r(3p-2) - x(3p-1) - x(3p-2)
//
// Note that x(3p-1) is already solved. So, we get x(3p-2) and x(3p-3). Next,
// let p := p-1 and repeat the same procedure to obtain all x values.
//

// Lastly, I need to get x(i) for i > 3p.


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

    int solution[size] = { 0 };

    // Derive truth[i] for i = 2, 5, 8, ...
    int x = 0;

    for (size_t i = 2; i < size; i += 3) {
        x = rolling_sums[i - 1] - rolling_sums[i - 2] + x;
        solution[i] = x;
    }

    size_t const p = (size - 1) / 3 * 3;

    solution[p] = 0; // assumption

    for (size_t q = p; q >= 3; q -= 3) {
        solution[q - 2] = rolling_sums[q - 1] - solution[q - 1] - solution[q];
        solution[q - 3] = rolling_sums[q - 2] - solution[q - 2] - solution[q - 1];
    }

    puts("i\ttruth\tsolution");
    for (size_t i = 0; i < size; i++) {
        printf("%zu\t%d\t%d\n", i, truth[i], solution[i]);
    }
}
