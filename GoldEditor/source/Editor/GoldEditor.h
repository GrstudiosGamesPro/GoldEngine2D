#pragma once
#include <iostream>

#include "../../../source/EngineBehaviour/GoldEngineLib.h"
#include "../UI/UIManager.h"
#include "../Components/Data/SaveData.h"
#include <imguizmo/ImGuizmo.h>


class GoldEditor : public EngineBehaviour { 
public:
    static bool testMode;


    static GoldEditor* editor;
    UIManager* uiMaster;
    string ProjectPath = "";
    float cameraSpeed = 1;
    float sensitivity = 100.0f;
    bool firstClick = true;
    bool activeMouse;


    //Shader ourShader;

    void start() override;
    void draw() override;

    void update() override;


    void PostRender() override;

    void PreRender() override;

    void drawUI() override;

    void release() override;
};