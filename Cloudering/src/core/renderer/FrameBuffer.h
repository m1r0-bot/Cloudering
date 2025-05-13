#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

class FrameBuffer
{
public:
	FrameBuffer(float width, float height);
	~FrameBuffer();
	GLuint getFrameTexture() { return texture; };
	void RescaleFrameBuffer(float width, float height);
	void Bind(glm::vec3 bcg = glm::vec3(0.)) const;
	void Unbind() const;

private:
	GLuint fbo;
	GLuint texture;
	GLuint rbo;

	float width;
	float height;


	inline static GLint previouseViewport[4] = {0, 0, 0, 0};
};
