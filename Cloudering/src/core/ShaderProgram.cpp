#include "ShaderProgram.h"



GLuint ShaderProgram::CreateShaderProgram(std::vector<Shader> shaders) {

	GLuint shaderProgram = glCreateProgram();
	for(Shader shader : shaders)
		glAttachShader(shaderProgram, shader.pid);

	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	return shaderProgram;
}