#version 330

#pragma frag:bits 64
#pragma frag:canvas 500 500
#pragma frag:scale 1
#pragma frag:wrap repeat
#pragma frag:fps 30

uniform vec2 resolution;
uniform sampler2D sampler;
uniform int frame;
in vec2 texCoord;
out vec4 fragColor;

float potential(float phi);
uint posNoise(uvec2 pos, uint seed);
uint feistel(uint val, uvec4 key);
uint encode(uint x, uint k);

void main() {
    const uint seed = 1u;
    const float enhance = 10.0;
    const float mobility = 0.9;
    const float epsilon = 0.003;
    const float dx = 0.1;
    const float dt = 0.0005;

    vec2 texel = 1 / resolution;
    float u0 = texture(sampler, texCoord).a;
    float u1 = texture(sampler, texCoord + texel * vec2(+1,  0)).a;
    float u2 = texture(sampler, texCoord + texel * vec2( 0, +1)).a;
    float u3 = texture(sampler, texCoord + texel * vec2(-1,  0)).a;
    float u4 = texture(sampler, texCoord + texel * vec2( 0, -1)).a;
    float u11 = texture(sampler, texCoord + texel * vec2(+2,  0)).a;
    float u22 = texture(sampler, texCoord + texel * vec2( 0, +2)).a;
    float u33 = texture(sampler, texCoord + texel * vec2(-2,  0)).a;
    float u44 = texture(sampler, texCoord + texel * vec2( 0, -2)).a;

    float pot_ddx = (potential(u1) - 2 * potential(u0) + potential(u3)) / pow(dx, 2);
    float pot_ddy = (potential(u2) - 2 * potential(u0) + potential(u4)) / pow(dx, 2);

    // Long-range correlation seems to require this term.
    float u_ddddx = (u11 - 4 * u1 + 6 * u0 - 4 * u3 + u33) / pow(dx, 4);
    float u_ddddy = (u22 - 4 * u2 + 6 * u0 - 4 * u4 + u44) / pow(dx, 4);

    float u = u0 + dt * mobility * (pot_ddx + pot_ddy - epsilon * (u_ddddx + u_ddddy));

    if (frame <= 1) {
        uvec2 pos = uvec2(texCoord * resolution);
        uint noise = posNoise(pos, seed);
        u = 0.01 + 0.98 * float(step(32768u, noise));
    }

    u = clamp(u, 0.03, 0.97);

    fragColor.rgb = 0.5 + enhance * (vec3(u, u, u) - 0.5);
    fragColor.a = u;
}

float potential(float phi) {
    const float chi = 1.5;
    return (1 - 2 * phi) * chi + log(phi) - log(1 - phi);
}

// Generate random 16-bit uint for given pixel point.
uint posNoise(uvec2 pos, uint seed) {
    uint code_x = pos.x + uint(resolution.x) * pos.y;
    uint code_y = pos.y + uint(resolution.y) * pos.x;
    uint code = (code_x ^ code_y) & 0xFFFFu;

    uint s1 = (seed >> 8) & 0xFFu;
    uint s2 = seed & 0xFFu;
    uvec4 key = uvec4(uvec2(s1, s2) ^ pos, uvec2(s2, s1) ^ pos);

    return feistel(code, key);
}

// 16-bit Feistel cipher with length-4 key.
uint feistel(uint val, uvec4 key) {
    uint s = (val >> 8) & 0xFFu;
    uint t = val & 0xFFu;
    uint prevT;

    prevT = t;
    t = s ^ (encode(t, key.x) & 0xFFu);
    s = prevT;

    prevT = t;
    t = s ^ (encode(t, key.y) & 0xFFu);
    s = prevT;

    prevT = t;
    t = s ^ (encode(t, key.z) & 0xFFu);
    s = prevT;

    prevT = t;
    t = s ^ (encode(t, key.w) & 0xFFu);
    s = prevT;

    return (t << 8) | s;
}

uint encode(uint x, uint k) {
    return (x * k) ^ k;
}
