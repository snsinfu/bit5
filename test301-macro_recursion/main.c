// Generate X(1), X(2), ..., X(N).

// This does not work because recursion is prohibited in preprocessor macro
// expansion. Also, there is no stopping condition.
#define GENERATE(N, X) \
    GENERATE(N - 1, X) \
    X(N)


#include <stdio.h>

int main(void)
{
#define X(I) int a##I = I;
    GENERATE(5, X)
#undef X

#define X(I) printf("%p\n", (void *) &a##I);
    GENERATE(5, X)
#undef X
}
