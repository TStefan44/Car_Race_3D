#pragma once
#include "car.h"
#include "track.h"

namespace tema2 {
	enum class DIR {BACK = -1, FORWARD = 1};

	class EnemyCar : public Car {
	public:
		EnemyCar(DIR carDir, tema2::Track* currentTrack);
		EnemyCar();

		void update(float deltaTimeSeconds = 1) override;
		void setCurrentPoint(int curerntPoint);

	private:
		void calcNewPoints();
		void calcFirstPoints();
		void calcAngleDir();
		void updatePoints();

	private:
		DIR carDir;
		int currentPoint;
		int maxPoint;
		glm::vec3 initialDir;
	};
}