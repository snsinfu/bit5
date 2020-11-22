// Divisibility rules.
//
// https://www.johndcook.com/blog/2020/11/10/test-for-divisibility-by-13/
// https://news.ycombinator.com/item?id=25165323
//
// This program reads two lines of input, the first one contains a (possibly
// large) integer and the second one containns a digit. Then, it checks if
// the first number is divisible by the second number and prints "yes" if it
// is and "no" otherwise.

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum
{
    max_digits = 10000,
};

struct problem
{
    char   dividend[max_digits + 2]; // +2 for newline and nul
    size_t num_digits;
    int    divisor;
};

static int read(struct problem *problem);
static int solve(struct problem const *problem);
static int check_2(char const *digits, size_t num_digits);
static int check_3(char const *digits, size_t num_digits);
static int check_4(char const *digits, size_t num_digits);
static int check_5(char const *digits, size_t num_digits);
static int check_6(char const *digits, size_t num_digits);
static int check_7(char const *digits, size_t num_digits);
static int check_8(char const *digits, size_t num_digits);
static int check_9(char const *digits, size_t num_digits);
static int digit_to_int(char digit);


int main(void)
{
    struct problem problem;

    if (read(&problem) < 0) {
        return 1;
    }

    if (solve(&problem)) {
        puts("yes");
    } else {
        puts("no");
    }

    return 0;
}

static int read(struct problem *problem)
{
    assert(problem != NULL);

    if (fgets(problem->dividend, sizeof problem->dividend, stdin) == NULL) {
        fprintf(stderr, "error: failed to read dividend: %s\n", strerror(errno));
        return -1;
    }

    size_t length = strlen(problem->dividend);

    if (length < 1 || problem->dividend[length - 1] != '\n') {
        fprintf(stderr, "error: dividend line is not terminated\n");
        return -1;
    }

    // We only need digits; trim newline.
    length--;

    if (length == 0) {
        fprintf(stderr, "error: dividend line is empty\n");
        return -1;
    }

    for (size_t i = 0; i < length; i++) {
        if (!isdigit(problem->dividend[i])) {
            fprintf(stderr, "error: dividend line contains non-digit character\n");
            return -1;
        }
    }

    problem->num_digits = length;

    if (scanf("%d", &problem->divisor) != 1) {
        fprintf(stderr, "error: failed to read divisor line: %s\n", strerror(errno));
        return -1;
    }

    if (problem->divisor < 1 || problem->divisor >= 10) {
        fprintf(stderr, "error: unexpected divisor\n");
    }

    return 0;
}

static int solve(struct problem const* problem)
{
    assert(problem != NULL);

    switch (problem->divisor) {
    case 1:
        return 1;

    case 2:
        return check_2(problem->dividend, problem->num_digits);

    case 3:
        return check_3(problem->dividend, problem->num_digits);

    case 4:
        return check_4(problem->dividend, problem->num_digits);

    case 5:
        return check_5(problem->dividend, problem->num_digits);

    case 6:
        return check_6(problem->dividend, problem->num_digits);

    case 7:
        return check_7(problem->dividend, problem->num_digits);

    case 8:
        return check_8(problem->dividend, problem->num_digits);

    case 9:
        return check_9(problem->dividend, problem->num_digits);

    default:
        assert(0);
    }
}

static int check_2(char const *digits, size_t num_digits)
{
    return digit_to_int(digits[num_digits - 1]) % 2 == 0;
}

static int check_3(char const *digits, size_t num_digits)
{
    int sum = 0;
    for (size_t i = 0; i < num_digits; i++) {
        sum += digit_to_int(digits[i]);
    }
    return sum % 3 == 0;
}

static int check_4(char const *digits, size_t num_digits)
{
    // Check the number formed by the last two digits.
    return atoi(digits + num_digits - 2) % 4 == 0;
}

static int check_5(char const *digits, size_t num_digits)
{
    return digits[num_digits - 1] == '0' || digits[num_digits - 1] == '5';
}

static int check_6(char const *digits, size_t num_digits)
{
    return check_2(digits, num_digits) && check_3(digits, num_digits);
}

static int check_7(char const *digits, size_t num_digits)
{
    int sum = 0;
    int sign = 1;

    for (size_t end = num_digits; end > 0; ) {
        char group[4] = {0, 0, 0, 0};
        size_t start = (end < 3 ? 0 : end - 3);
        memcpy(group, digits + start, end - start);

        sum += sign * atoi(group);
        sign = -sign;

        end = start;
    }

    return sum % 7 == 0;
}

static int check_8(char const *digits, size_t num_digits)
{
    // Check the number formed by the last three digits.
    return atoi(digits + num_digits - 3) % 8 == 0;
}

static int check_9(char const *digits, size_t num_digits)
{
    int sum = 0;
    for (size_t i = 0; i < num_digits; i++) {
        sum += digit_to_int(digits[i]);
    }
    return sum % 9 == 0;
}

static int digit_to_int(char digit)
{
    assert(digit >= '0' && digit <= '9');
    return digit - '0';
}
