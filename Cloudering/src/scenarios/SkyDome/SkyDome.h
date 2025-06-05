#pragma once
#include <glad/glad.h>
#include "../Scenario.h"
#include "../../core/ShaderProgram.h"
#include "../../core/renderer/Camera.h"
#include "../Noise/PerlinNoise.h"
#include "../Noise/WorleyNoise.h"
#include <vector>


class SkyDome : public Scenario {
public:
	SkyDome();
	void RenderControlerGUI() override;
	void RenderScene() override;

private:
	void GenerateNoises();

	Camera& cam = Camera::getInstance();
	GLuint VAO = 0;
	GLuint shaderProgram = 0;

	vec3 sunPos = vec3(0, 1, 0);
	int numOfSamples = 16;
	int numOfLightSamples = 8;

	float L = 0.56;
	float k1 = 0.1;
	float k2 = 0.2;
	float m = 2.5;

	float cloudMin = 0.557;
	float cloudMax = 1.;

	float raymarch_2sun_count = 25.;
	float raymarch_2sun_size = 1.;
	float ext_f = 0.01;

	std::vector<std::unique_ptr<Noise>> noises;

	// This is just to make the class diagram show the dependency
	Noise* _noises;
};