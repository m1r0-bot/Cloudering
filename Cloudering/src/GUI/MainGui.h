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


	void ShowSidebar(std::vector<Component>& components);
	std::vector<Component> v = { {"first"}, {"second"}, {"third"}, {"fourth"} };
};