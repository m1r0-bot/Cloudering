#pragma once
#include <glad/glad.h>
#include "Window.h"
#include "customBlocks/BlockFactory.h"
#include "customBlocks/M1Gui.h"
#include <vector>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <fstream>
#include <json/json.h>

struct Component {
	std::string name;
};

class MainGui {
public:
	MainGui(Window &window);
	void MainLoop(double deltaTime);

private:

	Window& window;

	void InitializeNewFrame();
	void MainMenuBar();
	void MainControlPanel();
	void RendererFrame();
	void DrawImGuiUI();

	void HandleKeyBinds();
	void SaveProjectToJSON();
	void LoadProjectFromJson();
};