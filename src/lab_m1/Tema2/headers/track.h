#pragma once
#include "object.h"

namespace tema2 {
	class Track : public Object {
	public:
		Track(float scaleX, float scaleZ);
		Track();
		~Track();

		std::vector<std::vector<glm::vec3>> getARPData();

	private:
		void createDataFromShape();
		Mesh* createTrack(const std::string& name);


	public:
		std::vector<std::vector<glm::vec3>> ARPData;

	private:
		float height;
		int minWidth;
		int maxWidth;

		float scaleX;
		float scaleZ;

		std::vector<glm::vec2> trackShape;
		std::vector<VertexFormat> vertexData;
		std::vector<unsigned int> indicesData;
	};
}