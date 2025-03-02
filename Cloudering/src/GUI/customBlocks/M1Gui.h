#pragma once
#include "imgui.h"
#include <string>
#include <vector>
#include <memory>

class Block;


class M1Gui {
public:
	static bool MovableCollapsingHeader(std::string label, int id, bool* destroy);

	static std::vector<std::unique_ptr<Block>> GUIComponents;

private:
	static int getIndex(int id);
};