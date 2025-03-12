#include "Shader.h"

Json::Value Shader::Serialize() const {
	 Json::Value json;

	 json["shaderPath"] = path;
	 json["shaderType"] = static_cast<int>(type);

	 return json;
}

void Shader::Deserialize(const Json::Value& json) {
	if (!json.isMember("shaderPath") || !json.isMember("shaderType"))
		throw std::runtime_error("Invalid JSON format");

	path = json["shaderPath"].asString();
	isSelected = !path.empty();

	type = static_cast<GLenum>(json["shaderType"].asInt());
}

std::string Shader::LoadShaderSource() {

	std::ifstream shaderFile(path);
	std::stringstream shaderStream;

	if (shaderFile) {
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
	}
	else {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path << std::endl;
	}

	return shaderStream.str();
}

GLuint Shader::CompileShader() {
	std::string source = LoadShaderSource();
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

void Shader::RenderUI() {
	size_t pos = path.find_last_of("\\");
	std::string name = (pos == std::string::npos) ? "no " + Type2Str(type) + " file" : path.substr(pos + 1);

	pos = name.find_last_of(".");
	std::string ext = (pos == std::string::npos) ? "" : name.substr(pos);

	auto width = std::min(150.0f, ImGui::GetContentRegionAvail().x - 30);
	if (ImGui::Button(name.c_str(), ImVec2(width, 17))) {

	}
	ImGui::SameLine();
	if (ImGui::SmallButton(("+##" + Type2Str(type)).c_str())) {
		SelectShader();
	}
	ImGui::SameLine();
	ImGui::Text("(%s)", ext.c_str());
}