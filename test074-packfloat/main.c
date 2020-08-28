#include <assert.h>
#include <math.h>
#include <stdio.h>


struct vec2 {
    float x;
    float y;
};

typedef struct vec2 vec2;


vec2  encode(float x);
float decode(vec2 v);
vec2  quantize(vec2 v);
vec2  unquantize(vec2 v);
float clamp(float x, float a, float b);


int main()
{
    float x = -0.12345F;
    vec2 v = quantize(encode(x));
    float y = decode(unquantize(v));

    assert(fabs(x - y) < 0.0001);

    printf("%.7g\n", x);
    printf("%.7g\n", y);
}


/*
 * Encode value of x into a low precision vector.
 */
vec2 encode(float x)
{
    x = clamp(x, -1, 1);
    x = 0.5F + 0.5F * x;

    float ex = 256 * x;
    float v1 = 1.0F / 256 * trunc(ex);
    float v2 = ex - trunc(ex);

    return (vec2) { v1, v2 };
}

/*
 * Decode value encoded in a low precision vector.
 */
float decode(vec2 v)
{
    float x = v.x + 1.0F / 256 * v.y;
    x = -1 + 2 * x;
    return x;
}

/*
 * Quantize the components of v to 8-bit integers.
 */
vec2 quantize(vec2 v)
{
    return (vec2) {trunc(v.x * 256), trunc(v.y * 256)};
}

/*
 * Reverses `quantize(v)`.
 */
vec2 unquantize(vec2 v)
{
    return (vec2) {v.x / 256, v.y / 256};
}

/*
 * Clamps x to interval [a,b].
 */
float clamp(float x, float a, float b)
{
    if (x < a) {
        return a;
    }
    if (x > b) {
        return b;
    }
    return x;
}
