#pragma once
#include "HUDS/SceneUI.h"
#include "HUDS/BarMenuUI.h"
#include "HUDS/HierarchyUI.h"
#include "HUDS/Assets.h"
#include "HUDS/Inspector.h"
#include "HUDS/RightClickUI.h"

#include "../Inspector/ScriptEditor.h"

class UIManager {
public:
	static UIManager* instance;
	SceneUI* sceneui;
	BarMenuUI* mainmenuui;
	HierarchyUI* hierarhcyui;
	InspectorUI* inspectorui;
	AssetsUI* assetsui;
	RightClickUI* rightClickui;
	ScriptEditor* editorScript;


	void start();

	void draw();

	void update();

	void fixupdate();

	void lateupdate();

	void createScriptEditor (Entity* owner);

	~UIManager();
};