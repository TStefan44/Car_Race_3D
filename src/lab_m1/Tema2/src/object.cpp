#include "../headers/object.h"

using namespace std;
using namespace tema2;

Object::Object(float posX, float posY, float posZ)
	: posX(posX), posY(posY), posZ(posZ) {}

Object::Object(float posX, float posZ)
	: Object(posX, 0, posZ) {}

Object::Object() : Object(0, 0) {};

void Object::setPosition(float X, float Z, float deltaTimeSeconds) {
	posX = X;
	posZ = Z;
}

void Object::update(float deltaTimeSeconds) {}

std::vector<Mesh*> Object::getComponentMeshes() {
	return componentMeshes;
}

std::vector<glm::mat4> Object::getMatrixComp() {
	return matrixComp;
}

void Object::addMesh(Mesh* mesh) {
	componentMeshes.push_back(mesh);
}