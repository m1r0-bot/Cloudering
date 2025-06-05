#include "NishitaDobashi_Scenario.h"


NishitaDobashi_Scenario::NishitaDobashi_Scenario() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Position vec3
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	pid = ShaderProgram::CreateShaderProgram({
		Shader("assets/shaders/cloudMetaball.vert", GL_VERTEX_SHADER),
		Shader("assets/shaders/cloudMetaball.frag", GL_FRAGMENT_SHADER)
		});
}

void NishitaDobashi_Scenario::RenderControlerGUI() {
	ca.RenderControlerGUI();
}

void NishitaDobashi_Scenario::RenderScene() {
	static std::vector<Metaball> cloudVoxels = {};
	if (ImGui::IsKeyPressed(ImGuiKey_N, true))
	{
		ca.TimeStep(); //ca is Cellular Automaton

		cloudVoxels = ca.getAllCldPos();

		//order from back to front
		std::sort(cloudVoxels.begin(), cloudVoxels.end(), [&](const Metaball& a, const Metaball& b) {
			float da = glm::length(cam.getEye() - a.pos);
			float db = glm::length(cam.getEye() - b.pos);
			return da > db;
			});

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, cloudVoxels.size() * sizeof(float) * 3, cloudVoxels.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//rendering
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_POINT_SPRITE);

	glUseProgram(pid);

	glUniformMatrix4fv(glGetUniformLocation(pid, "view"), 1, GL_FALSE, value_ptr( cam.getViewMatrix() ));
	glUniformMatrix4fv(glGetUniformLocation(pid, "projection"), 1, GL_FALSE, value_ptr( cam.getProjMatrix() ));
	glUniform1f(glGetUniformLocation(pid, "pointSize"), 100.0f * 30);

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, cloudVoxels.size());
	glBindVertexArray(0);

	glDisable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_BLEND);

}