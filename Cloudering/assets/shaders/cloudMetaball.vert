#version 330

layout(location = 0) in vec3 vertPos;

uniform mat4 view;
uniform mat4 projection;
uniform float pointSize;

out float height;

void main() {
    vec4 viewSpacePos = view * vec4(vertPos, 1.0);
    gl_Position = projection * viewSpacePos;
    height = 1.-vertPos.y/8.;
    gl_PointSize = pointSize / -viewSpacePos.z;
}