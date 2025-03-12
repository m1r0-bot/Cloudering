#include "ShaderProgram.h"



void ShaderProgram::CreateShaderProgram() {
	if (!vert.isSelected || !frag.isSelected) {
		std::cout << "missing vertex and fragment shaders!!" << std::endl;
		return;
	}

	bool tes = tesc.isSelected && tese.isSelected;
	bool geo = geom.isSelected;

	GLuint vertShader = vert.CompileShader();
	GLuint tescShader = tes ? tesc.CompileShader() : 0;
	GLuint teseShader = tes ? tese.CompileShader() : 0;
	GLuint geomShader = geo ? geom.CompileShader() : 0;
	GLuint fragShader = frag.CompileShader();

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	if (tes) glAttachShader(shaderProgram, tescShader);
	if (tes) glAttachShader(shaderProgram, teseShader);
	if (geo) glAttachShader(shaderProgram, geomShader);
	glAttachShader(shaderProgram, fragShader);

	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertShader);
	if (tes) glDeleteShader(tescShader);
	if (tes) glDeleteShader(teseShader);
	if (geo) glDeleteShader(geomShader);
	glDeleteShader(fragShader);

	glDeleteProgram(pid);
	pid = shaderProgram;
}


Json::Value ShaderProgram::Serialize() const {
	Json::Value json;

	json["vert"] = vert.Serialize();
	json["tesc"] = tesc.Serialize();
	json["tese"] = tese.Serialize();
	json["geom"] = geom.Serialize();
	json["frag"] = frag.Serialize();

	return json;
}

void ShaderProgram::Deserialize(const Json::Value& json) {
	if (!json.isMember("vert") || !json.isMember("tesc") || !json.isMember("tese") || !json.isMember("geom") || !json.isMember("frag"))
		throw std::runtime_error("Invalid JSON format");

	vert.Deserialize(json["vert"]);
	tesc.Deserialize(json["tesc"]);
	tese.Deserialize(json["tese"]);
	geom.Deserialize(json["geom"]);
	frag.Deserialize(json["frag"]);
}

void ShaderProgram::RenderUI() {
	vert.RenderUI();
	tesc.RenderUI();
	tese.RenderUI();
	geom.RenderUI();
	frag.RenderUI();

	if (ImGui::Button("Compile")) {
		CreateShaderProgram();
	}
	ImGui::SameLine();
	ImGui::Text("pid: %u", pid);
}