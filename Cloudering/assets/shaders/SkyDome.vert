#version 430 core

layout(location = 0) in vec2 uv;

out vec3 fragPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 ndc = vec4(uv*2-1, 1, 1);
    mat4 n_view = view;
    n_view[3].xyz = vec3(0);
    vec4 worldPos = inverse(projection * n_view) * ndc;
    worldPos /= worldPos.w;
    worldPos.y = -worldPos.y;

    fragPos = normalize(worldPos.xyz);

    gl_Position = ndc;
}