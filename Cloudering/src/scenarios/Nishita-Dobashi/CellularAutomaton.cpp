#include "CellularAutomaton.h"

void CellularAutomaton::RenderControlerGUI() {
	if (ImGui::CollapsingHeader("Automaton Size")) {
		ImGui::Text("DON'T TOUCH Y, NOT IMPLEMENTED YET XD");
		if (ImGui::DragInt("dim X", &dimX, 1, 0, 128) ||
			ImGui::DragInt("dim Y", &dimY, 1, 0, 32) ||
			ImGui::DragInt("dim Z", &dimZ, 1, 0, 128))
		{
			cld = std::vector<uint8_t>(dimX * dimZ, 0);
			hum = std::vector<uint8_t>(dimX * dimZ, 0);
			act = std::vector<uint8_t>(dimX * dimZ, 0);
			/*for (int i = 0; i < dimZ * dimX; i++) {
				hum.push_back(std::rand());
				act.push_back(std::rand());
			}*/
		}
	}

	if (ImGui::CollapsingHeader("Probabilities")) {
		ImGui::SliderFloat("Cloud Extinction",   &P_ext, 0, 1);
		ImGui::SliderFloat("Vapor Probability",  &P_hum, 0, 1);
		ImGui::SliderFloat("Phase Trans. Prob.", &P_act, 0, 1);
	}

	if (ImGui::CollapsingHeader("Ellipsoid")) {
		ImGui::Checkbox("Enable Ellipsoid", &enableEllipse);
		ImGui::DragFloat3("pos", &ellipsoids[0].pos.x);
		ImGui::DragFloat3("dim", &ellipsoids[0].dim.x);
	}
}

void CellularAutomaton::TimeStep() {
	auto f_act = [&](int i)
		{
			int x = i % dimX;
			int z = i / dimX;
			uint8_t out = (act[i] << 1) | (act[i] << 2) | (act[i] >> 1);
			out |= (x > 0) ? act[i - 1] : 0;
			out |= (x > 1) ? act[i - 2] : 0;
			out |= (x < dimX-1) ? act[i + 1] : 0;
			out |= (x < dimX-2) ? act[i + 2] : 0;

			out |= (z > 0) ? act[i - dimX] : 0;
			out |= (z > 1) ? act[i - dimX*2] : 0;
			out |= (z < dimZ-1) ? act[i + dimX] : 0;
			out |= (z < dimZ-2) ? act[i + dimX*2] : 0;


			return out;
		};

	auto rnd = []() {
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		};

	auto IS = [&](int bits, float p_init, int i, bool flip = false) 
		{
			uint8_t out = 0;
			float P = p_init;
			int x = i % dimX;
			int z = i / dimX;

			for (int y = 0; y < bits; y++) {
				if (enableEllipse) {
					float r = evaluateEllipsoids({ x, y, z });
					P = (flip ? r : 1 - r) * p_init;
				}
				out = (out << 1) | (flip ? rnd() > P : rnd() < P);
			}
			return out;
		};

	for (int i = 0; i < dimX * dimZ; i++) {
		auto prevHum = hum[i];
		auto prevAct = act[i];
		auto prevCld = cld[i];

		//cloud Growth
		hum[i] =  prevHum & ~prevAct;
		cld[i] =  prevCld |  prevAct;
		act[i] = ~prevAct &  prevHum & f_act(i);

		//cloud Extinction
		cld[i] = cld[i] & IS(sizeof(cld[i]) * 8, P_ext, i, true);
		hum[i] = hum[i] | IS(sizeof(hum[i]) * 8, P_hum, i);
		act[i] = act[i] | IS(sizeof(act[i]) * 8, P_act, i);
	}
}

std::vector<Metaball> CellularAutomaton::getAllCldPos() {
	std::vector<Metaball> out;
	for (int i = 0; i < dimX * dimZ; i++) {
		int x = i % dimX;
		int z = i / dimX;
		for (int y = 0; y < sizeof(cld[i]) * 8; y++) {
			if (cld[i] & (1 << y))
				out.push_back({ { x, y, z } });
		}
	}
	return out;
}

float CellularAutomaton::evaluateEllipsoids(glm::vec3 p) {
	float out = 1;
	for (Ellipsoid e : ellipsoids) {
		out = min(out, e.Evaluate(p));
	}
	return out;
}