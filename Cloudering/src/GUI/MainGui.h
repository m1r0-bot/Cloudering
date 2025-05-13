#pragma once
#include <glad/glad.h>
#include "Window.h"
#include "../core/renderer/Renderer.h"
#include "../scenarios/ScenarioFactory.h"
#include <vector>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <fstream>
#include <windows.h>

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

	void MouseCameraHandeler();
	void HandleShortcuts();
};