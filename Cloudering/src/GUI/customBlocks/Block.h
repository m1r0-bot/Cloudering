#pragma once
#include <imgui.h>
#include <string>
#include <vector>
#include <json/json.h>
#include "M1Gui.h"

enum EnumBlockType
{
	CAMERA,
	SHADER,
};

class Block {
private:
	static int nextId;
	int id;

public:
	Block() : id(nextId++) {}
	virtual void RenderUI() = 0;
	virtual Json::Value Serialize() const = 0;
	virtual void Deserialize(const Json::Value& json) = 0;
	virtual EnumBlockType GetType() const = 0;
	int getId() { return id; }

	static void ResetId() { nextId = 0; }



	bool isRenaming = false;

};