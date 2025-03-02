#pragma once
#include "Block.h"


class Camera_Block : public Block {
public:
	void RenderUI() override {
		ImGui::PushID(getId());
		static bool destroy = false;
		if (M1Gui::MovableCollapsingHeader(name, getId(), &destroy)) {
			ImGui::InputFloat3("position", pos.data());
			ImGui::InputFloat3("rotation", rotation.data());
			ImGui::Spacing();
			ImGui::SliderFloat("FOV", &FOV, 1, 180);
		}
		ImGui::PopID();
	}

	Json::Value Serialize() const override {
		Json::Value json;

		json["type"] = static_cast<int>(GetType());

		return json;
	}

	void Deserialize(const Json::Value& json) override {

	}

	EnumBlockType GetType() const override {
		return CAMERA;
	}



private:
	//todo replace it by Camera::GetInstance().pos / rotation / FOV / etc.
	std::string name = "Camera";
	std::vector<float> pos = {0, 0, 0};
	std::vector<float> rotation = { 0, 0, 0 };
	float FOV = 90;
};