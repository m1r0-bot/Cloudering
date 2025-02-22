#pragma once
#include <glad/glad.h>
#include "Window.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
};