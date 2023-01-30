#include "../headers/enemy_car.h"

using namespace tema2;

EnemyCar::EnemyCar(DIR carDir, tema2::Track* currentTrack) : Car(currentTrack), carDir(carDir) {
	currentPoint = 0;
	maxPoint = ARPData.size();
	calcFirstPoints();
	calcAngleDir();
	initialDir = glm::normalize(nextP - currentP);
}

EnemyCar::EnemyCar() {}

void EnemyCar::update(float deltaTimeSeconds) {
	glm::vec3 D;
	glm::mat4 model = glm::mat4(1);

	if (glm::length(nextP - currentP) < 2.f && glm::normalize(nextP - currentP) != initialDir) {
		updatePoints();
		calcAngleDir();
		initialDir = glm::normalize(nextP - currentP);
	}

	D = nextP - currentP;
	D.y = 0;
	D = glm::normalize(D);
	currentP += D * carSpeed * deltaTimeSeconds;
	currentP.y = 1.f;
	model = glm::translate(model, currentP);
	model = glm::scale(model, glm::vec3(5, 5, 5));
	initial_rot = quadRotation(-D, 8.f * deltaTimeSeconds);
	model *= glm::toMat4(initial_rot);
	matrixComp[0] = model;
}

void EnemyCar::setCurrentPoint(int currentPoint) {
	this->currentPoint = currentPoint;
	calcFirstPoints();
}

void EnemyCar::updatePoints() {
	switch (carDir) {
	case DIR::BACK:
		currentPoint -= 1;
		if (currentPoint < 0)
			currentPoint = maxPoint - 1;
		break;
	case DIR::FORWARD:
		currentPoint += 1;
		if (currentPoint == maxPoint)
			currentPoint = 0;
	}

	calcNewPoints();
}

void EnemyCar::calcNewPoints() {
	glm::vec3 P;
	float dist = (rand() % 2 == 0) ? -(rand() % 20) : (rand() % 20);
	currentP = nextP;

	switch (carDir) {
	case DIR::BACK :
		P = (currentPoint == 0) ? ARPData[maxPoint - 1][2] : ARPData[currentPoint - 1][2];
		nextP = (currentPoint == 0)? ARPData[maxPoint - 1][3] : ARPData[currentPoint - 1][3];
		break;
	case DIR::FORWARD :
		P = (currentPoint == maxPoint - 1) ? ARPData[0][2] : ARPData[currentPoint + 1][2];
		nextP = (currentPoint == maxPoint - 1)? ARPData[0][3] : ARPData[currentPoint + 1][3];
	}
	nextP += dist * P;
}

void EnemyCar::calcFirstPoints() {
	currentP = ARPData[currentPoint][3];

	switch (carDir) {
	case DIR::BACK:
		nextP = (currentPoint == 0) ? ARPData[maxPoint - 1][3] : ARPData[currentPoint - 1][3];
		break;
	case DIR::FORWARD:
		nextP = (currentPoint == maxPoint - 1) ? ARPData[0][3] : ARPData[currentPoint + 1][3];
	}
}

void EnemyCar::calcAngleDir() {
	glm::vec3 D = ARPData[currentPoint][4];
	D = nextP - currentP;
	D.y = 0;
	D = glm::normalize(D);
	angleDir = atan(D.x / D.z);
}