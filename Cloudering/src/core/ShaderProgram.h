#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"



class ShaderProgram {
public:
	~ShaderProgram() {
		glDeleteProgram(pid);
	}

	void CreateShaderProgram();
	Json::Value Serialize() const;
	void Deserialize(const Json::Value& json);
	void RenderUI();

	GLuint pid;
	Shader vert{ GL_VERTEX_SHADER };
	Shader tesc{ GL_TESS_CONTROL_SHADER };
	Shader tese{ GL_TESS_EVALUATION_SHADER };
	Shader geom{ GL_GEOMETRY_SHADER };
	Shader frag{ GL_FRAGMENT_SHADER };
};