#pragma once
#include <string>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>


class Shader {
public:
	Shader(std::string path, GLenum type) : path(path), type(type){
		pid = CompileShader();
	}
	~Shader() {
		glDeleteShader(pid);
	}
	GLuint pid;
private:
	GLuint CompileShader();
	std::string LoadShaderSource(const std::string& filePath);

	std::string path;
	GLenum type;
};