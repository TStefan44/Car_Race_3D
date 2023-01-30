#pragma once
#include "object.h"
#include "track.h"

namespace tema2 {
	class Car : public Object {
	public:
		Car(tema2::Track* currentTrack, glm::vec3 carColor, float carSpeed);
		Car(tema2::Track* currentTrack, glm::vec3 carColor);
		Car(tema2::Track* currentTrack);
		Car();

		bool checkCollision(Car B);
		glm::vec3 getColor();
		void setCarSpeed(float carSpeed);
		void setColor(glm::vec3 carColor);
		glm::vec3 getCurrentP();
		glm::quat quadRotation(glm::vec3 dir, float angularSpeed);
		float getCarSpeed();

	protected:
		tema2::Track* currentTrack;
		std::vector<std::vector<glm::vec3>> ARPData;
		glm::vec3 carColor;
		float carSpeed;
		float angleDir;
		float radius;
		glm::vec3 currentP;
		glm::vec3 nextP;
		glm::quat initial_rot;
	};
}