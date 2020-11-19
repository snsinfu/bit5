#include <stdio.h>


double Σ(double const *x, size_t n)
{
    double sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += x[i];
    }
    return sum;
}

int main(void)
{
    double σ = 1.0;
    double μ = 1.5;

    printf("%g\n", σ / μ);

    double x[] = {1.2, 3.4, 5.6, 7.8};
    size_t n = sizeof x / sizeof *x;

    printf("%g\n", Σ(x, n));

    return 0;
}
