#include "../headers/car.h"

using namespace tema2;

Car::Car(tema2::Track* currentTrack, glm::vec3 carColor, float carSpeed) : currentTrack(currentTrack), carColor(carColor), carSpeed(carSpeed) {
	initial_rot = glm::quat(glm::vec3_up, glm::vec3_forward);
	if (currentTrack != NULL)
		ARPData = currentTrack->getARPData();
	matrixComp.push_back(glm::mat4(1));
	radius = 5.f;
}

Car::Car(tema2::Track* currentTrack) : Car(currentTrack, glm::vec3(1, 1, 1)) {}

Car::Car(tema2::Track* currentTrack, glm::vec3 carColor) : Car(currentTrack, carColor, 0) {}

Car::Car() : Car(NULL, glm::vec3(1, 1, 1)) {}

glm::quat Car::quadRotation(glm::vec3 dir, float angularSpeed) {
	glm::quat targetRot = glm::quatLookAt(dir, glm::vec3_up);
	float rotSlerpFactor = glm::clamp(angularSpeed, 0.f, 1.f);
	return glm::slerp(initial_rot, targetRot, rotSlerpFactor);
}

bool Car::checkCollision(Car B) {
	glm::vec3 AB = B.currentP - currentP;
	float lenght = glm::length(AB);
	if (lenght < radius + B.radius)
		return true;
	return false;
}

glm::vec3 Car::getColor() {
	return carColor;
}

void Car::setCarSpeed(float carSpeed) {
	this->carSpeed = carSpeed;
}

void Car::setColor(glm::vec3 carColor) {
	this->carColor = carColor;
}

glm::vec3 Car::getCurrentP() {
	return currentP;
}

float Car::getCarSpeed() {
	return carSpeed;
}