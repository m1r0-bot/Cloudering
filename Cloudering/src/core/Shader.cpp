#include "Shader.h"


std::string Shader::LoadShaderSource(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << std::endl;
		return "";
	}

	std::stringstream shaderStream;
	std::string line;
	while (std::getline(file, line)) {
		if (line.find("#include") != std::string::npos) {
			size_t firstQuote = line.find('\"');
			size_t lastQuote = line.find('\"', firstQuote + 1);
			if (firstQuote != std::string::npos && lastQuote != std::string::npos) {
				std::string includePath = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
				std::string includeFullPath = std::filesystem::path(path).parent_path().string() + "/" + includePath;
				shaderStream << LoadShaderSource(includeFullPath);  // Recursive include
			}
		}
		else {
			shaderStream << line << '\n';
		}
	}
	return shaderStream.str();
}

GLuint Shader::CompileShader() {
	std::string source = LoadShaderSource(path);
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	// Check for compilation errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}