#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Scenario.h"
#include "../../core/ShaderProgram.h"
#include "../../core/renderer/Camera.h"
#include "CellularAutomaton.h"
#include <vector>


class NishitaDobashi_Scenario : public Scenario {
public:
	NishitaDobashi_Scenario();
	void RenderControlerGUI() override;
	void RenderScene() override;

private:
	CellularAutomaton ca;
	Camera& cam = Camera::getInstance();

	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint pid = 0; //shader pid
};