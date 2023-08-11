#pragma once
#include "box2d/box2d.h"
#include "../../source/EngineBehaviour/GoldEngineLib.h"

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
	float density = 1.0f;
	float friction = 0.3f;
	float Mass = 1;
	bool isStatic;
	bool isTrigger = false;

	bool FreezeX = false;
	bool FreezeY = false;

	void init	() override;
	void update () override;
	void draw	() override;
	void clean  () override;

	void changeState(bool val);
	void UpdateCollisions();

	void triggerOn  (Entity* enterEntity);
	void triggerOff (Entity* enterEntity);

	//LOCAL BODY CONFIG
	bool usedTrigger;
};