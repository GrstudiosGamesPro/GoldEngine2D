#include "BoxCollider.h"
#include "nlohmann/json.hpp"

using namespace nlohmann;

void BoxCollider::init() {
	mPhysics = SceneManager::GetSceneManager()->OpenScene->mPhysics;
	shape = mPhysics->createShape(physx::PxBoxGeometry(boxSize.x, boxSize.y, boxSize.z), *SceneManager::GetSceneManager()->OpenScene->mMaterial, 1);

	//PxTransform newTransform; // Crea una nueva transformaci�n

	//PxVec3 newPosition(entity->transform->Position.x, entity->transform->Position.y, entity->transform->Position.z); // Cambia estas coordenadas seg�n la posici�n deseada
	//newTransform.p = newPosition;
	//shape->setLocalPose(newTransform);
}

void BoxCollider::update() {
	physx::PxGeometry* newGeometry = nullptr;
	newGeometry = new physx::PxBoxGeometry (boxSize.x, boxSize.y, boxSize.z);
	shape->setGeometry (*newGeometry);
}

void BoxCollider::draw() {

}

void BoxCollider::clean() {
	shape->release();
}

std::string BoxCollider::serialize() {
	json componentData;
	componentData["boxsizex"] = boxSize.x;
	componentData["boxsizey"] = boxSize.y;
	componentData["boxsizez"] = boxSize.z;

	return componentData.dump();
}

void BoxCollider::deserialize(std::string g, std::string path) {
	json componentData = json::parse(g);

	boxSize.x = componentData["boxsizex"];
	boxSize.y = componentData["boxsizey"];
	boxSize.z = componentData["boxsizez"];
}
