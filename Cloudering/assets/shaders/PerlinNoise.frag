#version 430 core

in vec2 fragUV;
out vec4 FragColor;

uniform vec4 gridSize;

#define TWO_PI 6.28318530718

float rand (vec2 st) {
    return fract(sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123);
}

vec2 hash22(vec2 p) 
{
    float r = rand(p) * TWO_PI;
    return vec2(cos(r), sin(r));
}

float Perlin(vec2 p, float _mod)
{
    vec2 i = floor(p * _mod);
    vec2 f = fract(p * _mod);
    
    vec2 c00 = hash22(mod(i + vec2(0., 0.), _mod));
    vec2 c01 = hash22(mod(i + vec2(0., 1.), _mod));
    vec2 c10 = hash22(mod(i + vec2(1., 0.), _mod));
    vec2 c11 = hash22(mod(i + vec2(1., 1.), _mod));
    
    vec2 p00 = f - vec2(0., 0.);
    vec2 p01 = f - vec2(0., 1.);
    vec2 p10 = f - vec2(1., 0.);
    vec2 p11 = f - vec2(1., 1.);
    
    vec2 u = f*f*(3.0-2.0*f);
    
    float a = mix(dot(c00, p00), dot(c01, p01), u.y);
    float b = mix(dot(c10, p10), dot(c11, p11), u.y);
    
    return mix(a, b, u.x);
}

void main()
{
    float r = bool(gridSize.x) ? Perlin(fragUV, gridSize.x) : 0.;
    float g = bool(gridSize.y) ? Perlin(fragUV, gridSize.y) : 0.;
    float b = bool(gridSize.z) ? Perlin(fragUV, gridSize.z) : 0.;
    float a = bool(gridSize.w) ? Perlin(fragUV, gridSize.w) : 0.;

    FragColor = vec4(r, g, b, a)*.5+.5;
}