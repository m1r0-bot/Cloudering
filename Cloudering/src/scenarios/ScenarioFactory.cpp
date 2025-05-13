#include "ScenarioFactory.h"
#include "TestTriangle/TestTriangle.h"
#include "Nishita-Dobashi/NishitaDobashi_Scenario.h"
#include "SkyDome/SkyDome.h"
#include "Horizon/HorizonZeroDawn.h"

std::shared_ptr<Scenario> ScenarioFactory::currentScenario;
std::unordered_map<EnumScenario, std::shared_ptr<Scenario>> ScenarioFactory::cache;

std::shared_ptr<Scenario> ScenarioFactory::Create(EnumScenario scenario) {
	auto it = cache.find(scenario);
	if (it != cache.end()) {
		return it->second;
	}

	std::shared_ptr<Scenario> instance = nullptr;
	switch (scenario) {
	case TEST: instance = std::make_shared<TestTriangle>(); break;
	case METABALLS: instance = std::make_shared<NishitaDobashi_Scenario>(); break;
	case SKYDOME: instance = std::make_shared<SkyDome>(); break;
	case HORIZON_0_DAWN: instance = std::make_shared<HorizonZeroDawn>(); break;
	default: 
		return nullptr;
	}

	if (instance) {
		cache[scenario] = instance;
	}
	return instance;
}

void ScenarioFactory::setCurrentScene(EnumScenario scenario) {
	std::shared_ptr new_scenario = Create(scenario);
	if (new_scenario == nullptr) {
		std::cerr << "SCENARIO ENUM <" << (int)scenario << "> IS NOT IMPLEMENTED" << std::endl;
		return;
	}
	currentScenario = new_scenario;
}