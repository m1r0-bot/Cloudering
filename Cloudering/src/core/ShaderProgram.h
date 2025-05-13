#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "vector"



class ShaderProgram {
public:
	static GLuint CreateShaderProgram(std::vector<Shader> shaders);
};