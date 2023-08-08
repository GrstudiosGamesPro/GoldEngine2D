#pragma once
#include "../Camera/Camera2D.h"
#include "../ECS/ECS.h"
#include <vector>
#include <iostream>

using namespace std;


class Scene
{
public:
	Camera* worldCamera = new Camera();
	//Manager ObjectsScene;
	std::vector<Entity*> objectsInScene = std::vector<Entity*>();

	void start();
	void update();
	void release();

};