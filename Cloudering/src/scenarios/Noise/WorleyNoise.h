#include "Noise.h"

class WorleyNoise : public Noise {
public:
	WorleyNoise(int width, int height, glm::vec4 gridSize, bool repeat = true)
		: Noise(width, height), repeat(repeat), gridSize(gridSize) {}

protected:
	void ProcessShaderProgram() override {
		GLuint pid = ShaderProgram::CreateShaderProgram({
			Shader("assets/shaders/WorleyNoise.vert", GL_VERTEX_SHADER),
			Shader("assets/shaders/WorleyNoise.frag", GL_FRAGMENT_SHADER)
			});

		glUseProgram(pid);

		glUniform4f(glGetUniformLocation(pid, "gridSize"), gridSize.x, gridSize.y, gridSize.z, gridSize.w); // 0 means: "ignore this channel"
	}

	bool repeat;
	glm::vec4 gridSize;
};