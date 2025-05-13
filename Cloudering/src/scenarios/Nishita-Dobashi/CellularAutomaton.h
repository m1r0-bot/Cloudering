#pragma once
#include <glm/glm.hpp>
#include <imgui.h>
#include <vector>
#include <inttypes.h>
#include <algorithm>
#include "../../core/renderer/Camera.h"


struct Metaball {
	glm::vec3 pos;
};

struct Ellipsoid {
	glm::vec3 dim;
	glm::vec3 pos;
	
	float Evaluate(vec3 p) {
		return pow((p.x - pos.x) / dim.x, 2) + pow((p.y - pos.y) / dim.y, 2) + pow((p.z - pos.z) / dim.z, 2);
	}
};

class CellularAutomaton {
public:
	CellularAutomaton() {
		for (int i = 0; i < dimZ*dimX; i++) {
			hum.push_back( std::rand() );
			act.push_back( std::rand() );
		}
	}
	void RenderControlerGUI();
	void TimeStep();
	std::vector<Metaball> getAllCldPos();

	int dimX = 16;
	int dimY = 8;
	int dimZ = 16;

	float P_ext = 0.1;
	float P_hum = 0.1;
	float P_act = 0.1;

private:
	std::vector<uint8_t> hum;
	std::vector<uint8_t> act;
	std::vector<uint8_t> cld = std::vector<uint8_t>(dimX * dimZ, 0);

	std::vector<Ellipsoid> ellipsoids = { {{8, 4, 4}, {8, 4, 8}} };
	float evaluateEllipsoids(glm::vec3 p);

	bool enableEllipse = false;


};