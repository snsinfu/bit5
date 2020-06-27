#include <cstddef>
#include <cstdio>
#include <cstdlib>

using std::size_t;

int main()
{
    size_t n = 32768;
    int* p = static_cast<int*>(::operator new[](n * sizeof(int)));

    for (size_t i = 0; i < n; i++) {
        if (p[i] != 0) {
            std::printf("%zu\t%d\n", i, p[i]);
        }
    }

    ::operator delete[](p);
}
