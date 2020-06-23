// Assumptions:
//   unsigned  32-bit
//   float     IEEE 754 single-precision

#include <assert.h>
#include <stdio.h>
#include <string.h>


float add(float a, float b);


int main(void)
{
    float a = 1.23F;
    float b = 4.56F;
    float c = add(a, b);

    printf("%g\n", c);

    return 0;
}


struct ieee754
{
    unsigned sign;
    unsigned exponent;
    unsigned mantissa;
};

static struct ieee754 dissect(float a)
{
    unsigned bits;
    memcpy(&bits, &a, sizeof a);

    struct ieee754 repr = {
        .sign = bits >> 31 & 1,
        .exponent = bits >> 23 & 0xff,
        .mantissa = bits & 0x7fffff,
    };
    return repr;
}

static float synthesize(struct ieee754 repr)
{
    unsigned bits = (repr.sign << 31) | (repr.exponent << 23) | repr.mantissa;
    float value;
    memcpy(&value, &bits, sizeof value);
    return value;
}

static struct ieee754 add_repr(struct ieee754 a, struct ieee754 b)
{
    // I do not handle subtraction here.
    assert(a.sign == b.sign);

    if (a.exponent < b.exponent) {
        struct ieee754 tmp = a;
        a = b;
        b = tmp;
    }
    assert(a.exponent >= b.exponent);

    // Set implicit 24-th bit before adding mantissa.
    unsigned a_mant = 0x800000 | a.mantissa;
    unsigned b_mant = (0x800000 | b.mantissa) >> (a.exponent - b.exponent);
    unsigned sum_mant = a_mant + b_mant;
    unsigned sum_exp = a.exponent;

    // Fix mantissa/exponent and clear implicit 24-th bit.
    unsigned carries = sum_mant >> 24;
    sum_mant >>= carries;
    sum_exp += carries;
    sum_mant -= 0x800000;

    return (struct ieee754) {a.sign, sum_exp, sum_mant};
}


float add(float a, float b)
{
    return synthesize(add_repr(dissect(a), dissect(b)));
}
