#pragma once
#include "car.h"
#include "track.h"
#include "lab_camera.h"

namespace tema2 {
	class PlayerCar : public Car {
	public:
		PlayerCar(glm::vec3 currentP, implemented::Camera *sceneCamera, tema2::Track* currentTrack);
		PlayerCar();

		void update(float deltaTimeSeconds = 1) override;
		void accelerateCar(float accVal);
		void slowCar(float slow);
		bool carOnTrack();

		void setMaxSpeed(float maxSpeed);
		void setAcc(float acc);
		float getAcc();

	private:
		float triangleArea(glm::vec2 A, glm::vec2 B, glm::vec2 C);
		bool triangleAreaAproach(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 P);

	private:
		implemented::Camera *sceneCamera;
		float maxSpeed;
		float acc;
	};
}