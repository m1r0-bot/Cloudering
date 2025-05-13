#pragma once
#include <glad/glad.h>
#include "../Scenario.h"
#include <vector>
#include "../../core/ShaderProgram.h"


class TestTriangle : public Scenario {
public:
	TestTriangle();
	void RenderControlerGUI() override;
	void RenderScene() override;

private:
	GLuint VAO = 0;
	GLuint shaderProgram = 0;
};