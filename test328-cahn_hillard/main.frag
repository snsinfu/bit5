#version 330

#pragma frag:canvas 600 600
#pragma frag:scale 1
#pragma frag:wrap repeat
#pragma frag:fps 60

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
    const float enhance = 1.0;
    const uint seed = 23456u;
    const float D = 1.0;
    const float dx = 0.1;
    const float dt = 0.0005;

    vec2 texel = 1 / resolution;
    float u0 = texture(sampler, texCoord).a;
    float u1 = texture(sampler, texCoord + texel * vec2(+1,  0)).a;
    float u2 = texture(sampler, texCoord + texel * vec2( 0, +1)).a;
    float u3 = texture(sampler, texCoord + texel * vec2(-1,  0)).a;
    float u4 = texture(sampler, texCoord + texel * vec2( 0, -1)).a;

    float pot_ddx = (potential(u1) - 2 * potential(u0) + potential(u3)) / (dx * dx);
    float pot_ddy = (potential(u2) - 2 * potential(u0) + potential(u4)) / (dx * dx);
    float u = u0 + dt * D * (pot_ddx + pot_ddy);

    {
        uvec2 pos = uvec2(texCoord * resolution);
        uint noise = posNoise(pos, seed);
        float rand = -0.5 + float(step(32768u, noise));
        u += sqrt(dt * D) * rand;
    }

    if (frame <= 1) {
        uvec2 pos = uvec2(texCoord * resolution);
        uint noise = posNoise(pos, seed);
        u = 0.4 + 0.2 * float(step(32768u, noise));
    }

    u = clamp(u, 0.03, 0.97);

    fragColor.rgb = 0.5 + enhance * (vec3(u, u, u) - 0.5);
    fragColor.a = u;
}

float potential(float phi) {
    const float chi = 1.2;
    return (1 - 2 * phi) * chi + log(phi) - log(1 - phi);
}

// Generate random 16-bit uint for given pixel point.
uint posNoise(uvec2 pos, uint seed) {
    uint code = (pos.x + uint(resolution.x) * pos.y) & 0xFFFFu;
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
