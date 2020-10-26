module;
import <cmath>;

export module math;

export double sigmoid(double x)
{
    return 1 / (1 + std::exp(x));
}
