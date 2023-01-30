#include "../headers/track.h"

using namespace tema2;

Track::Track(float scaleX, float scaleZ) : Object(0, 0), scaleX(scaleX), scaleZ(scaleZ) {
	trackShape = {
		glm::vec2(-300, 0), glm::vec2(-289, 26), glm::vec2(-269, 44), glm::vec2(-250, 50),
		glm::vec2(-232, 38), glm::vec2(-223, 18), glm::vec2(-202, 16), glm::vec2(-184, 18),
		glm::vec2(-166, 29), glm::vec2(-150, 50), glm::vec2(-136, 65), glm::vec2(-128, 89),
		glm::vec2(-138, 107), glm::vec2(-156, 111), glm::vec2(-166, 125), glm::vec2(-171, 149),
		glm::vec2(-153, 172), glm::vec2(-130, 184), glm::vec2(-112, 189), glm::vec2(-92, 189),
		glm::vec2(-65, 186), glm::vec2(-56, 172), glm::vec2(-56, 156), glm::vec2(-54, 134),
		glm::vec2(-40, 116), glm::vec2(-23, 110), glm::vec2(0, 110), glm::vec2(20, 116),
		glm::vec2(46, 116), glm::vec2(63, 110), glm::vec2(60, 94), glm::vec2(49, 75),
		glm::vec2(43, 55), glm::vec2(43, 36), glm::vec2(65, 24), glm::vec2(86, 18),
		glm::vec2(109, 16), glm::vec2(128, 16), glm::vec2(157, 16), glm::vec2(176, 16),
		glm::vec2(195, 16), glm::vec2(228, 17), glm::vec2(250, 16), glm::vec2(274, 16),
		glm::vec2(298, 12), glm::vec2(313, 12),	glm::vec2(327, 11), glm::vec2(336, -8),
		glm::vec2(329, -21), glm::vec2(314, -21), glm::vec2(297, -24), glm::vec2(281, -25),
		glm::vec2(259, -29), glm::vec2(233, -28), glm::vec2(214, -27), glm::vec2(196, -30),
		glm::vec2(177, -30), glm::vec2(160, -30), glm::vec2(146, -31), glm::vec2(131, -30),
		glm::vec2(114, -33), glm::vec2(97, -34), glm::vec2(81, -34), glm::vec2(64, -38),
		glm::vec2(49, -57), glm::vec2(50, -82), glm::vec2(59, -93), glm::vec2(78, -111),
		glm::vec2(95, -114), glm::vec2(115, -123), glm::vec2(130, -129), glm::vec2(150, -150),
		glm::vec2(146, -164), glm::vec2(137, -181), glm::vec2(124, -196), glm::vec2(100, -200),
		glm::vec2(85, -202), glm::vec2(61, -195), glm::vec2(41, -182), glm::vec2(43, -166),
		glm::vec2(32, -158), glm::vec2(5, -163), glm::vec2(-5, -185), glm::vec2(-7, -214),
		glm::vec2(-22, -227), glm::vec2(-56, -234), glm::vec2(-78, -230), glm::vec2(-96, -220),
		glm::vec2(-112, -204), glm::vec2(-122, -187), glm::vec2(-127, -168), glm::vec2(-127, -148),
		glm::vec2(-109, -122), glm::vec2(-86, -103), glm::vec2(-86, -86), glm::vec2(-93, -66),
		glm::vec2(-111, -48), glm::vec2(-141, -46), glm::vec2(-163, -60), glm::vec2(-187, -69),
		glm::vec2(-207, -76), glm::vec2(-228, -78), glm::vec2(-252, -77), glm::vec2(-283, -70),
		glm::vec2(-292, -53), glm::vec2(-299, -29),
	};
	minWidth = 30;
	maxWidth = 30;
	height = 1.f;

	createDataFromShape();
	componentMeshes.push_back(createTrack("Track"));
}

Track::Track() : Track(1, 1) {};

Track::~Track() {};

void Track::createDataFromShape() {
	if (trackShape.size() <= 1) {
		std::cerr << "Track Shape doesn't have enough points!\n";
		return;
	}

	int size = trackShape.size();
	for (int i = 0; i < size; i++) {
		// Extract points
		glm::vec3 P1 = glm::vec3(trackShape[i].x, 0, trackShape[i].y);
		glm::vec3 P2 = (i + 1 != size)?
					   glm::vec3(trackShape[i + 1].x, 0, trackShape[i + 1].y) :
					   glm::vec3(trackShape[0].x, 0, trackShape[0].y);
		P1.x *= scaleX; P1.z *= scaleZ;
		P2.x *= scaleX; P2.z *= scaleZ;
		
		// Calculate dir vector (P) and perpendicular vector on direction (P)
		glm::vec3 D = P2 - P1;
		glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3_up));

		// Calculate A, R
		int distA = minWidth + rand() % (1 + maxWidth - minWidth);
		int distR = minWidth + rand() % (1 + maxWidth - minWidth);
		// Interpolate for more points (A, R)
		glm::vec3 A, R;
		for (float j = 1.f; j >= 0; j -= 0.05f) {

			A = j * P1 + (1.f - j) * P2 - P * (float)distA; A.y = height;
			R = j * P1 + (1.f - j) * P2 + P * (float)distR; R.y = height;

			// Save new points in vertexData / Construct vertexData
			vertexData.push_back(VertexFormat(A));
			vertexData.push_back(VertexFormat(R));
		}

		// Save local track width, and P for future usage
		std::vector<glm::vec3> data = { A, R, P, P1, D};
		ARPData.push_back(data);
	}

	// Construct indicesData
	size = vertexData.size();
	for (int i = 0; i <= size - 4; i += 2) {
		//A1A2R1 trinagle
		indicesData.push_back(i);
		indicesData.push_back(i + 2);
		indicesData.push_back(i + 1);

		//A2R2R1 triangle
		indicesData.push_back(i + 2);
		indicesData.push_back(i + 3);
		indicesData.push_back(i + 1);
	}

	indicesData.push_back(size - 2);
	indicesData.push_back(0);
	indicesData.push_back(size - 1);

	indicesData.push_back(0);
	indicesData.push_back(1);
	indicesData.push_back(size - 1);
}

Mesh* Track::createTrack(const std::string& name) {
	Mesh* track = new Mesh(name);
	track->InitFromData(vertexData, indicesData);
	return track;
}

std::vector<std::vector<glm::vec3>> Track::getARPData() {
	return ARPData;
}