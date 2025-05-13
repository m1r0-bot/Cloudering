#include "SkyDome.h"

SkyDome::SkyDome() {
	std::vector<float> far_plane{
		0, 0,
		1, 0,
		0, 1,
		1, 1
	};

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, far_plane.size() * sizeof(float), far_plane.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

	shaderProgram = ShaderProgram::CreateShaderProgram({
		Shader("assets/shaders/SkyDome.vert", GL_VERTEX_SHADER),
		Shader("assets/shaders/SkyDome.frag", GL_FRAGMENT_SHADER) 
		});

	GenerateNoises();
}

void SkyDome::GenerateNoises() {
	noises.push_back(std::make_unique<PerlinNoise>(1024, 1024, glm::vec4(40., 100., 150., 200.), true));
	noises.back().get()->GenerateTexture();
	noises.push_back(std::make_unique<WorleyNoise>(1024, 1024, glm::vec4(50., 75., 100., 150.), true));
	noises.back().get()->GenerateTexture();
}

void SkyDome::RenderControlerGUI() {
	if(ImGui::CollapsingHeader("SkyDome")) {
		ImGui::DragFloat3("Sun Position", &sunPos[0], 0.1, 0, 0, "%.1f");
		ImGui::SliderInt("View Samples Count", &numOfSamples, 0, 64);
		ImGui::SliderInt("Light Samples Count", &numOfLightSamples, 0, 64);

	}

	if (ImGui::CollapsingHeader("Cloud attributes")) {
		ImGui::Text("Lt = (n * S)L");
		ImGui::DragFloat("L", &L, 0.01, 0, 0, "%.2f");
		ImGui::Text("Ls = [k1 + k2(v * S)]^m * L");
		ImGui::SliderFloat("k1", &k1, 0, 1);
		ImGui::SliderFloat("k2", &k2, 0, 1);
		ImGui::DragFloat("m", &m, 0.01, 0, 0, "%.2f");
		
		ImGui::Spacing();

		ImGui::SliderFloat("cloudMin", &cloudMin, -1., 1.);
		ImGui::SliderFloat("cloudMax", &cloudMax, -1., 1.);

		ImGui::Spacing();
		ImGui::Text("cloud extinction");
		ImGui::DragFloat("raymarch_2sun_count", &raymarch_2sun_count, 0.01, 0, 0, "%.2f");
		ImGui::DragFloat("raymarch_2sun_size", &raymarch_2sun_size, 0.01, 0, 0, "%.2f");
		ImGui::DragFloat("ext_f", &ext_f, 0.01, 0, 0, "%.2f");
		
	}

	if (ImGui::CollapsingHeader("GeneratedTextures")) {
		for (auto& noise : noises) {
			ImGui::Image(
				(ImTextureID)noise.get()->getTexture(),
				ImVec2(128, 128)
			);
		}
	}
}

void SkyDome::RenderScene() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glUseProgram(shaderProgram);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, value_ptr(cam.getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, value_ptr(cam.getProjMatrix()));

	vec3 sunDir = glm::normalize(sunPos);
	glUniform3f(glGetUniformLocation(shaderProgram, "sunDir"), sunDir.x, sunDir.y, sunDir.z);
	glUniform1ui(glGetUniformLocation(shaderProgram, "numSamples"), numOfSamples);
	glUniform1ui(glGetUniformLocation(shaderProgram, "numSamplesLight"), numOfLightSamples);

	glUniform1f(glGetUniformLocation(shaderProgram, "RAYMARCH_2SUN_COUNT"), raymarch_2sun_count);
	glUniform1f(glGetUniformLocation(shaderProgram, "RAYMARCH_2SUN_SIZE"), raymarch_2sun_size);
	glUniform1f(glGetUniformLocation(shaderProgram, "EXTINCTION_F"), ext_f);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, noises[0].get()->getTexture());
	glUniform1i(glGetUniformLocation(shaderProgram, "perlinNoise"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noises[1].get()->getTexture());
	glUniform1i(glGetUniformLocation(shaderProgram, "worleyNoise"), 2);


	glUniform1f(glGetUniformLocation(shaderProgram, "L"), L);
	glUniform1f(glGetUniformLocation(shaderProgram, "k1"), k1);
	glUniform1f(glGetUniformLocation(shaderProgram, "k2"), k2);
	glUniform1f(glGetUniformLocation(shaderProgram, "m"), m);
	glUniform1f(glGetUniformLocation(shaderProgram, "cloudMin"), cloudMin);
	glUniform1f(glGetUniformLocation(shaderProgram, "cloudMax"), cloudMax);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}