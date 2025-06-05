#include "HorizonZeroDawn.h"

HorizonZeroDawn::HorizonZeroDawn() {
	std::vector<float> far_plane{
		-1,-1, 1,
		 1,-1, 1,
		-1, 1, 1,
		 1, 1, 1
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, far_plane.size() * sizeof(float), far_plane.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

	shaderProgram = ShaderProgram::CreateShaderProgram({
		Shader("assets/shaders/Horizon0Dawn/HorizonClouds.vert", GL_VERTEX_SHADER),
		Shader("assets/shaders/Horizon0Dawn/HorizonClouds.frag", GL_FRAGMENT_SHADER) 
		});

	GenerateNoises();

}

void HorizonZeroDawn::GenerateNoises() {
	lowFreqNoises = HorizonTextureGenerator::newTexture3D(Shader("assets/shaders/Horizon0Dawn/lowFreqNoises.comp", GL_COMPUTE_SHADER), glm::vec3(128));
	highFreqNoises = HorizonTextureGenerator::newTexture3D(Shader("assets/shaders/Horizon0Dawn/highFreqNoises.comp", GL_COMPUTE_SHADER), glm::vec3(32));
	weatherData = HorizonTextureGenerator::newTexture2D(Shader("assets/shaders/Horizon0Dawn/weatherData.comp", GL_COMPUTE_SHADER), glm::vec2(1024));
}

void HorizonZeroDawn::RenderControlerGUI() {
	if (ImGui::CollapsingHeader("SkyDome")) {
		ImGui::DragFloat3("Sun Position", &sunPos[0], 0.1, 0, 0, "%.1f");
	}
	if (ImGui::CollapsingHeader("clouds")) {
		ImGui::ColorPicker3("Sun Color", &sunCol[0]);
	}
}

void HorizonZeroDawn::RenderScene() {
	glUseProgram(shaderProgram);


	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, value_ptr(cam.getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, value_ptr(cam.getProjMatrix()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, lowFreqNoises);
	glUniform1i(glGetUniformLocation(shaderProgram, "lowFreqNoises"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, highFreqNoises);
	glUniform1i(glGetUniformLocation(shaderProgram, "highFreqNoises"), 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, weatherData);
	glUniform1i(glGetUniformLocation(shaderProgram, "weatherDataTex"), 2);


	vec3 sunDir = glm::normalize(sunPos);
	glUniform3f(glGetUniformLocation(shaderProgram, "SunDir"), sunDir.x, sunDir.y, sunDir.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "SunCol"), sunCol.x, sunCol.y, sunCol.z);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}