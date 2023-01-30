#include "../headers/player_car.h"

using namespace tema2;

PlayerCar::PlayerCar(glm::vec3 currentP, implemented::Camera *sceneCamera, tema2::Track* currentTrack) : Car(currentTrack), sceneCamera(sceneCamera) {
	this->currentP = currentP;
	carSpeed = 0.f;
	acc = 0.f;
	maxSpeed = 0.f;
}

PlayerCar::PlayerCar() : PlayerCar(glm::vec3(0), NULL, NULL) {}

void PlayerCar::update(float deltaTimeSeconds) {
	glm::mat4 model = glm::mat4(1);
	glm::vec3 dir = glm::normalize(sceneCamera->forward);
	dir.y = 0;
	float rotSpeed = glm::clamp(carSpeed / 50, 1.f, 10.f);

	currentP = sceneCamera->position + sceneCamera->distanceToTarget * dir;
	currentP.y = 2.f;

	model = glm::translate(model, currentP);
	model = glm::scale(model, glm::vec3(5, 5, 5));
	initial_rot = quadRotation(dir, rotSpeed * deltaTimeSeconds);
	model *= glm::toMat4(initial_rot);
	matrixComp[0] = model;
}

void PlayerCar::accelerateCar(float accVal) {
	carSpeed += accVal;
	if (carSpeed > maxSpeed)
		carSpeed = maxSpeed;
	if (carSpeed < -maxSpeed)
		carSpeed = -maxSpeed;
}

void PlayerCar::slowCar(float slow) {
	if (carSpeed > 0)
		carSpeed -= slow;
	else if (carSpeed < 0)
		carSpeed += slow;

	if (abs(carSpeed) <= 0.5f)
		carSpeed = 0;
}

float PlayerCar::triangleArea(glm::vec2 A, glm::vec2 B, glm::vec2 C) {
	glm::vec2 AB = B - A;
	glm::vec2 AC = C - A;
	float cross_product = AB.x * AC.y - AB.y * AC.x;
	return abs(cross_product);
}

bool PlayerCar::triangleAreaAproach(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 P) {
	float triangle_are = triangleArea(A, B, C);
	float area_sum = 0;
	area_sum += triangleArea(A, B, P);
	area_sum += triangleArea(A, C, P);
	area_sum += triangleArea(B, C, P);
	if (abs(triangle_are - area_sum) <= 0.1f)
		return true;
	return false;
}

bool PlayerCar::carOnTrack() {
	glm::vec3 A1, A2, R1, R2;
	int next;
	for (int i = 0; i < ARPData.size(); i++) {
		next = (i + 1 == ARPData.size()) ? 0 : i + 1;
		A1 = ARPData[i][0]; A2 = ARPData[next][0];
		R1 = ARPData[i][1]; R2 = ARPData[next][1];
		if (triangleAreaAproach(
			glm::vec2(A1.x, A1.z),
			glm::vec2(A2.x, A2.z),
			glm::vec2(R1.x, R1.z),
			glm::vec2(currentP.x, currentP.z)))
			return true;
		if (triangleAreaAproach(
			glm::vec2(A2.x, A2.z),
			glm::vec2(R2.x, R2.z),
			glm::vec2(R1.x, R1.z),
			glm::vec2(currentP.x, currentP.z)))
			return true;
	}
	return false;
}

void PlayerCar::setMaxSpeed(float maxSpeed) {
	this->maxSpeed = maxSpeed;
}

void PlayerCar::setAcc(float acc) {
	this->acc = acc;
}

float PlayerCar::getAcc() {
	return acc;
}