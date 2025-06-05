#pragma once
#include <memory>
#include "Scenario.h"
#include <iostream>
#include <unordered_map>

class ScenarioFactory {
public:
	static std::shared_ptr<Scenario> currentScenario;
	static bool isSelectedScenario() { return currentScenario.get() != nullptr; }
	static void setCurrentScene(EnumScenario scenario);

private:
	static std::unordered_map<EnumScenario, std::shared_ptr<Scenario>> cache;
	static std::shared_ptr<Scenario> Create(EnumScenario scenario);

	// This is just to make the class diagram show the dependency
	Scenario *currentscenario;

};