#pragma once
#include <iostream>
#include "box2d/box2d.h"
#include "../../source/EngineBehaviour/GoldEngineLib.h"

using namespace std;

struct b2Body;
struct b2PolygonShape;
struct b2FixtureDef;

class RigidBody : public Component
{
public:
	b2Body* body;
	b2PolygonShape* dynamicBox;
	b2FixtureDef* fixtureDef;
	b2Vec2 position;

	uint16 mask = 0x0001;
	uint16 group = 0x0001;

	float density = 25.0f;
	float friction = 0.3f;
	float Mass = 1;
	bool isStatic;
	bool isTrigger = false;

	bool FreezeX = false;
	bool FreezeY = false;

	bool ControlByParent;

	void init	() override;
	void update () override;
	void draw	() override;
	void clean  () override;

	void changeState(bool val);
	void UpdateCollisions();

	void triggerOn  (Entity* enterEntity);
	void triggerOff (Entity* enterEntity);
	void addForce   (glm::vec2);

	//LOCAL BODY CONFIG
	bool usedTrigger;


	string serialize() override;
	void deserialize(std::string g, std::string path = "") override;
};