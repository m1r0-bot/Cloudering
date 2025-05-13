#version 430 core

in vec2 fragUV;
out vec4 FragColor;

uniform vec4 gridSize;

//hash by Dave Hoshins: https://www.shadertoy.com/view/4djSRW
vec2 hash22(vec2 p) 
{
    vec3 p3 = fract(vec3(p.xyx) * 0.1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xx + p3.yz) * p3.zy);
}

float Worley(vec2 p, float _mod){
    vec2 i = floor(p * _mod);
    vec2 f = fract(p * _mod);
    
    float min_dist = 10.;
    
    for (float y = -1.; y <= 1.; y++){
        for (float x = -1.; x <= 1.; x++)
        {
            vec2 off = vec2(x, y);
            vec2 r = hash22(mod(i + off, _mod)) + off - f;
            
            float d = dot(r, r);
            
            min_dist = min(min_dist, d);
        }
    }
    min_dist = clamp(.0, 1., min_dist);
    return 1.-min_dist;
}

void main()
{
    float r = bool(gridSize.x) ? Worley(fragUV, gridSize.x) : 0.;
    float g = bool(gridSize.y) ? Worley(fragUV, gridSize.y) : 0.;
    float b = bool(gridSize.z) ? Worley(fragUV, gridSize.z) : 0.;
    float a = bool(gridSize.w) ? Worley(fragUV, gridSize.w) : 0.;

    FragColor = vec4(r, g, b, a);
}