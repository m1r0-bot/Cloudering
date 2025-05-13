#include "Renderer.h"

void Renderer::RenderScene(float width, float height) {
	if (width != this->width || height != this->height) {
		this->width = width;
		this->height = height;
		cam.updateProjMatrix();
		fb.RescaleFrameBuffer(width, height);
	}
	fb.Bind(glm::vec3(.7, .75, .8));

	if (ScenarioFactory::isSelectedScenario()) {
		ScenarioFactory::currentScenario.get()->RenderScene();
	}

	fb.Unbind();
}