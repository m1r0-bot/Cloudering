#pragma once
#include "glad/glad.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "../../scenarios/ScenarioFactory.h"


class Renderer {
public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	static Renderer& getInstance() {
		static Renderer instance;
		return instance;
	}

	GLuint getTexture() { return fb.getFrameTexture(); };
	void RenderScene(float width, float height);

private:
	Renderer() {
		cam.Init(&width, &height);
	}
	~Renderer() {}

	Camera& cam = Camera::getInstance();
	FrameBuffer fb = FrameBuffer(1920, 1080);

	float width = 1920;
	float height = 1080;
};