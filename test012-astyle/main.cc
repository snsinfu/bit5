#include <iostream>
#include <random>


template<int R, int C>
static constexpr int rows(double(& )[R][C])
{
    return R;
}

template<int R, int C>
static constexpr int cols(double(& )[R][C])
{
    return C;
}

int main()
{
    std::mt19937_64 random;
    std::uniform_real_distribution<double> uniform{0, 1};

    double matrix[50][50];

    for (int i = 0; i < rows(matrix); i++) {
        for (int j = 0; j < cols(matrix); j++) {
            matrix[i][j] = uniform(random);
        }
    }

    double sum = 0;

    for (int i = 0; i < rows(matrix); i++) {
        for (int j = 0; j < rows(matrix); j++) {
            sum += matrix[i][j];
        }
    }

    std::cout << sum << '\n';
}
