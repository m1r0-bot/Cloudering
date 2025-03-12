#pragma once
#include "imgui.h"
#include <string>
#include <vector>
#include <memory>
#include <json/json.h>

class Block;


class M1Gui {
public:
	static bool MovableCollapsingHeader(std::string &label, int id, bool* destroy, bool* isRenaming = nullptr);


	static void QueueToDestroy(int id) { destroyQueue.push_back(id); };
	static void DestroyQueuedComponents();
	static Json::Value ComponentsToJSON();
	static void JSONToComponents(Json::Value & components);
	static std::vector<std::unique_ptr<Block>> GUIComponents;

private:
	static int getIndex(int id);
	static std::vector<int> destroyQueue;
};