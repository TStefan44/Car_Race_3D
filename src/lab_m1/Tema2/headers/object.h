#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace tema2
{
	class Object
	{
	public:
		Object();
		Object(float posX, float posZ);
		Object(float posX, float posY, float posZ);

		virtual void setPosition(float X, float Z, float deltaTimeSeconds = 1);
		virtual void update(float deltaTimeSeconds = 1);

		std::vector<Mesh*> getComponentMeshes();
		void addMesh(Mesh *mesh);
		std::vector<glm::mat4> getMatrixComp();

	protected:
		float posX, posY, posZ;

		std::vector<Mesh*> componentMeshes;
		std::vector<glm::mat4> matrixComp;
	};
}