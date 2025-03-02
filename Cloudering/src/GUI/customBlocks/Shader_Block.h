#pragma once
#include "Block.h"
#include "..\..\core\ShaderProgram.h"


class Shader_Block : public Block {
public:
	void RenderUI() override {
		ImGui::PushID(getId());
		static bool destroy;
		if (M1Gui::MovableCollapsingHeader(displayName.c_str(), getId(), &destroy)) {
			sp.RenderUI();
		}

		ImGui::PopID();
	}

	Json::Value Serialize() const override {
		Json::Value json;
		
		json["type"] = static_cast<int>(GetType());
		json["displayName"] = displayName;

		json["shaderProgram"] = sp.Serialize();

		return json;
	}

	void Deserialize(const Json::Value& json) override {
		if (!json.isMember("displayName") || !json.isMember("shaderProgram")) {
			throw std::runtime_error("Invalid JSON format");
		}
		displayName = json["displayName"].asString();
		sp.Deserialize(json["shaderProgram"]);
	}

	EnumBlockType GetType() const override {
		return SHADER;
	}

private:
	std::string displayName = "Shader";
	ShaderProgram sp;
};