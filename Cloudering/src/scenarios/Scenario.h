#pragma once
#include <glad/glad.h>
#include "imgui.h"

enum EnumScenario
{
	TEST,
	METABALLS,
	SKYDOME,
	HORIZON_0_DAWN
};

class Scenario {
public:
	virtual void RenderControlerGUI() = 0;
	virtual void RenderScene() = 0;
	virtual void RenderMenuItems() {};
	virtual void HandleUserInput() {};

protected:
};