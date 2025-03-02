#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../tools/FileLoader.h"

class Window {
public:
	Window(int width, int height) : width(width), height(height)
	{
		Json::Value root;
		if (FileLoader::LoadJSON(root, "saves/lastSave")) {
			width = root["SCREEN"]["WIDTH"].asInt();
			height = root["SCREEN"]["HEIGHT"].asInt();
		}
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);

		window = glfwCreateWindow(width, height, "Cloudering", NULL, NULL);
		if (window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}

		glfwSetWindowUserPointer(window, this);

		glfwMakeContextCurrent(window);
		gladLoadGL();

		glViewport(0, 0, width, height);
	}

	~Window()
	{
		std::cout << "Window Closed" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	GLFWwindow* window;
	int width;
	int height;

};