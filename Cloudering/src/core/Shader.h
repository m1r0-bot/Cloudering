#pragma once
#include <string>
#include <glad/glad.h>
#include <json/json.h>
#include "../tools/FileLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "imgui.h"


class Shader {
public:
	Shader(GLenum ShaderType) : type(ShaderType) {}
	void SelectShader() {
		path = FileLoader::OpenFileDialog(Type2Str(type));
		isSelected = !path.empty();
	}

	void RemoveShader() {
		path = "";
		isSelected = false;
	}

	Json::Value Serialize() const;
	void Deserialize(const Json::Value& json);
	GLuint CompileShader();
	void RenderUI();

	bool isSelected = false;
	std::string path = "";
	GLenum type;
private:
	std::string LoadShaderSource();

	std::string Type2Str(GLenum shaderType) {
		switch (shaderType) {
		case GL_VERTEX_SHADER:
			return "vert";
		case GL_TESS_CONTROL_SHADER:
			return "tesc";
		case GL_TESS_EVALUATION_SHADER:
			return "tese";
		case GL_GEOMETRY_SHADER:
			return "geom";
		case GL_FRAGMENT_SHADER:
			return "frag";
		default:
			return "";
		}
	}
};