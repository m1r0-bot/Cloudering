#version 430 core

layout(location = 0) in vec3 position;

out vec2 fragUV;

void main() {
    fragUV = position.xy*.5+.5;
    gl_Position = vec4(position, 1.0);
}