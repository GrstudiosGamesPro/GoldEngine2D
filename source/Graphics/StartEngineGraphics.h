#pragma once
#include "../RequireLibs.h"
#include "../EngineBehaviour/EngineBehaviour.h"

class StartEngineGraphics
{
public:
	static StartEngineGraphics* instance;
	static GLFWwindow* window;
	static EngineBehaviour* engine;
	//Skybox* sky = new Skybox();

	float time = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	bool gameStarted;


	void create();
	void release();
	void update();
	void releasewindow();
	void StartEngine();

	void RenderImgui();
	void RenderOpenGL();

	//STATIC GLOBAL FUNCTIONS
	static void closeWindow();

	StartEngineGraphics* GetGraphics();
};