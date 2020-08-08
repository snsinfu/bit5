#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>


static void case_overflow();
static void case_valid_infinity();
static void case_underflow();


int
main(void)
{
    case_overflow();
    case_valid_infinity();
    case_underflow();
}

static void
case_overflow()
{
    const char *str = "1.23e456";

    errno = 0;
    char *end;
    double val = strtod(str, &end);
    int err = errno;

    assert(*end == '\0');
    assert(isinf(val));
    assert(err == ERANGE);
}

static void
case_valid_infinity()
{
    const char *str = "+infinity";

    errno = 0; // Really necessary to clear errno.
    char *end;
    double val = strtod(str, &end);
    int err = errno;

    assert(*end == '\0');
    assert(isinf(val));
    assert(err == 0);
}

static void
case_underflow()
{
    const char *str = "1.23e-456";

    errno = 0; // Really necessary to clear errno.
    char *end;
    double val = strtod(str, &end);
    int err = errno;

    assert(*end == '\0');
    assert(val == 0);
    assert(err == ERANGE);
}
