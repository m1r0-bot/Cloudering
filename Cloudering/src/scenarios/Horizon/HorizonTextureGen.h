#pragma once
#include "../../core/ShaderProgram.h"
#include <glm/glm.hpp>


class HorizonTextureGenerator {
public:
	static GLuint newTexture3D(Shader computeShader, glm::vec3 dimensions) {
		GLuint pid = ShaderProgram::CreateShaderProgram({ computeShader });

		GLuint texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_3D, texture);

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, dimensions.x, dimensions.y, dimensions.z, 0, GL_RGBA, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		glUseProgram(pid);
		glBindImageTexture(0, texture, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(dimensions.x / 8, dimensions.y / 8, dimensions.z / 8);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glDeleteProgram(pid);

		return texture;
	}

	static GLuint newTexture2D(Shader computeShader, glm::vec2 dimensions) {
		GLuint pid = ShaderProgram::CreateShaderProgram({ computeShader });

		GLuint texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dimensions.x, dimensions.y, 0, GL_RGBA, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glUseProgram(pid);
		glBindImageTexture(0, texture, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(dimensions.x / 8, dimensions.y / 8, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glDeleteProgram(pid);

		return texture;
	}
};