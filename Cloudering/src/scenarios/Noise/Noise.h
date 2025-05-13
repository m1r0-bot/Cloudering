#pragma once
#include <glad/glad.h>
#include "../../core/renderer/FrameBuffer.h"
#include "../../core/ShaderProgram.h"
#include <vector>



class Noise {
public:
	Noise(int width, int height)
		: fb(width, height) 
	{
		std::vector<float> points = {
		-1,-1, 1,
		 1,-1, 1,
		-1, 1, 1,
		 1, 1, 1
		};

		GLuint VBO;
		glCreateBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	GLuint getTexture() { return fb.getFrameTexture(); }

	void GenerateTexture() {
		fb.Bind();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		ProcessShaderProgram();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		fb.Unbind();
	}

protected:
	virtual void ProcessShaderProgram() = 0;

	FrameBuffer fb;
	GLuint VAO;
};