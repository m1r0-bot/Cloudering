#include "GUI/Window.h"
#include "GUI/MainGui.h"



int main() {
    Window window = Window(1280, 720);
	MainGui mainGui = MainGui(window);

	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window.window))
	{
		double currentTime = glfwGetTime();
		mainGui.MainLoop(currentTime - lastTime);
		lastTime = currentTime;

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
}
