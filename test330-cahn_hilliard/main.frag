#version 330

#pragma frag:bits 64
#pragma frag:canvas 200 200
#pragma frag:scale 3
#pragma frag:wrap repeat
#pragma frag:fps 30

uniform vec2 resolution;
uniform sampler2D sampler;
uniform int frame;
in vec2 texCoord;
out vec4 fragColor;

uint posNoise(uvec2 pos, uint seed);
uint feistel(uint val, uvec4 key);
uint encode(uint x, uint k);

float potential(float phi, float chi) {
    return (1 - 2 * phi) * chi + log(phi) - log(1 - phi);
}

void main() {
    const uint seed = 1u;
    const float enhance = 4.0;
    const float dt = 0.05;
    const float chi = 2;

    // chi = 1.8 shows coarse separation
    // chi = 2.0 shows clear, fine-structured separation

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

    float pot_ddx = potential(u1, chi) - 2 * potential(u0, chi) + potential(u3, chi);
    float pot_ddy = potential(u2, chi) - 2 * potential(u0, chi) + potential(u4, chi);
    float u = u0 + dt * (pot_ddx + pot_ddy);

    if (frame <= 1) {
        uvec2 pos = uvec2(texCoord * resolution);
        uint noise = posNoise(pos, seed);
        u = 0.01 + 0.98 * float(step(32768u, noise));
    }

    u = clamp(u, 0.03, 0.97);

    fragColor.rgb = 0.5 + enhance * (vec3(u, u, u) - 0.5);
    fragColor.a = u;
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
