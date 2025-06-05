#include "TestTriangle.h"

TestTriangle::TestTriangle() {
	std::vector<float> points = {
		-0.5f, -0.5f,  0.0f,
		0.5f, 0.0f,  0.0f,
		0.0f, 0.5f,  0.0f 
	};

	std::vector<float> colors = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};

	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

	GLuint colors_vbo;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	shaderProgram = ShaderProgram::CreateShaderProgram({
		Shader("assets/shaders/test.vert", GL_VERTEX_SHADER),
		Shader("assets/shaders/test.frag", GL_FRAGMENT_SHADER) 
		});
}

void TestTriangle::RenderControlerGUI() {

}

void TestTriangle::RenderScene() {
	glDisable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}