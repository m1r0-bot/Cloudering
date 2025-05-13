#pragma once
#include <glad/glad.h>
#include "../Scenario.h"
#include "../../core/renderer/Camera.h"
#include "../../core/ShaderProgram.h"
#include "HorizonTextureGen.h"
#include <vector>


class HorizonZeroDawn : public Scenario {
public:
	HorizonZeroDawn();
	void RenderControlerGUI() override;
	void RenderScene() override;

private:
	void GenerateNoises();

	Camera& cam = Camera::getInstance();
	GLuint VAO = 0;
	GLuint shaderProgram = 0;

	GLuint lowFreqNoises = 0;
	GLuint highFreqNoises = 0;
	GLuint weatherData = 0;

	glm::vec3 sunPos = vec3(0., 1., 0.);
};