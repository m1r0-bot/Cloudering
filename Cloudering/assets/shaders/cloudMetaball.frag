#version 330

out vec4 FragColor;

void main() {
    vec2 uv = 2. * gl_PointCoord - vec2(1.);
    
    float a = clamp(1. - dot(uv, uv), .0, 1.)*0.1;

    FragColor = vec4(vec3(1.), a);
}