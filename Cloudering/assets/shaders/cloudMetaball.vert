#version 330

layout(location = 0) in vec3 vertPos;

uniform mat4 view;
uniform mat4 projection;
uniform float pointSize;

void main() {
    vec4 viewSpacePos = view * vec4(vertPos, 1.0);
    gl_Position = projection * viewSpacePos;

    gl_PointSize = pointSize / -viewSpacePos.z;
}