#include "../headers/forest.h"

using namespace tema2;

Forest::Forest(tema2::Track *currentTrack) : Object(0, 0), currentTrack(currentTrack) {
	std::vector<std::vector<glm::vec3>> ARPData = currentTrack->getARPData();
	if (ARPData.size() == 0) {
		std::cerr << "EMPTY ARP DATA!!!!\n";
		return;
	}

	// Create forest
	for (int i = 0; i < ARPData.size(); i++) {
		std::vector<glm::vec3> data = ARPData[i];

		if (data.size() != 5) {
			std::cerr << "Error creating forest! ARPData element has " << data.size() << " elements!\n";
			for (glm::vec3 elem : data) {
				std::cerr << elem << "\n";
			}
			return;
		}
		
		// Extract data from ARP vector info
		glm::vec3 A = data[0];
		glm::vec3 R = data[1];
		glm::vec3 P = data[2];

		// Choose tree type
		// TODO: add  more trees =((
		trees.push_back(treeType::FIR);

		// Calculate tree position. Choose random position type A | R
		float distance = 30 + (rand() % 5 + 1);
		glm::vec3 position;
		glm::mat4 model = glm::mat4(1);
		if (rand() % 2 == 0) {
			// R case
			position = R + P * distance;
		}
		else {
			// A case
			position = A - P * distance;
		}

		// Special cases
		if ((i >= 44 && i <= 55) || (i >= 27 && i <= 36)) {
			position = R + P * (float)3 * distance;
		}

		if ((i >= 74 && i <= 83) || (i >= 4 && i <= 8)) {
			position = A - P * (float)3 * distance;
		}

		position.y = 5;
		
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(15, 20, 15));
		matrixComp.push_back(model);
	}
}

Forest::Forest() : Object() {}

std::vector<tema2::treeType> Forest::getTreesType() {
	return trees;
}