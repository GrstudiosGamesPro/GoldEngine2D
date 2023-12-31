#pragma once
#include "SceneUI.h"
#include "../../Editor/GoldEditor.h"
#include "../UIManager.h"
#include <imguizmo/ImGuizmo.h>
#include <glm/glm.hpp>
#include <iostream>
#include "../../Gizmos/GDGizmos.h"


using namespace ImGuizmo;
using namespace std;


ImGuizmo::MODE gizmoMode(ImGuizmo::LOCAL);
ImGuizmo::OPERATION gizmoOperation(ImGuizmo::TRANSLATE);

//Shader* pickingShader = nullptr;
GLuint pickingFramebuffer; // FBO para renderizar la textura de selecci�n
GLuint pickingTexture;     // Textura de selecci�n
GLuint pickingShaderProgram; // Programa de sombreado para la selecci�n

void SceneUI::start() {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, AppSettings::RenderWidth, AppSettings::RenderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is not complete!" << std::endl;



    glGenTextures(1, &colorAndDepthTexture);
    glBindTexture(GL_TEXTURE_2D, colorAndDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, AppSettings::RenderWidth, AppSettings::RenderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Crear y configurar el renderbuffer de profundidad
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, AppSettings::RenderWidth, AppSettings::RenderHeight);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glGenFramebuffers(1, &framebuffer);
}


void SceneUI::draw() {

#pragma region DRAW TEXTURE SCENE
    //ImDrawList* draw_list = ImGui::GetWindowDrawList();


    ImVec2 windowSize = ImVec2(AppSettings::RenderWidth, AppSettings::RenderHeight);

    // Dibujamos la imagen en la ventana de ImGui
    ImGui::SetNextWindowSize(windowSize);
    ImGui::PushID ("SceneUIView");
    windowOpen = true;

    ImGui::Begin("Scene Open", &windowOpen);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        //RenderSizeWindow = ImGui::GetWindowSize();
        ImGuizmo::SetDrawlist();

        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 offset = ImGui::GetWindowContentRegionMin();
        ImVec2 size = ImGui::GetContentRegionAvail();

        ImVec2 actualWindowSize = ImGui::GetWindowSize();
        float scaleFactor = std::min(actualWindowSize.x / windowSize.x, actualWindowSize.y / windowSize.y);
        imageSizeSCENE = ImVec2((AppSettings::RenderWidth) * scaleFactor, (AppSettings::RenderHeight) * scaleFactor);


        // Calculamos la posici�n para centrar la imagen en la ventana
        imagePosition = ImVec2((actualWindowSize.x - imageSizeSCENE.x) * 0.5f, (actualWindowSize.y - imageSizeSCENE.y) * 0.5f);

        ImGui::SetCursorPos(imagePosition);

        int WindowXSize = imageSizeSCENE.x;
        int WindowYSize = imageSizeSCENE.y;
        imageSizeSCENE.x = WindowXSize;
        imageSizeSCENE.y = WindowYSize;


        ImVec2 p = ImGui::GetCursorScreenPos();

        // Invertimos las coordenadas de textura en el eje Y antes de mostrar la imagen
        ImGui::Image((void*)(intptr_t)texture, ImVec2(WindowXSize, WindowYSize), ImVec2(ImVec2(0, 1)), ImVec2(ImVec2(1, 0)));
        if (!UIManager::instance->rightClickui->miniMenuOpen) {
            UIManager::instance->rightClickui->SceneHover = ImGui::IsItemHovered();
        }
        GoldEditor::editor->activeMouse = ImGui::IsItemHovered();


        if (!UIManager::instance->tileMapUI->EditionModeActive) {
#pragma region IMGUIZMO 


            if (UIManager::instance->inspectorui->ObjectSelectToInspector != nullptr) {
                bool ignoreGui = false;
                static ImGuizmo::MODE gizmoMode(ImGuizmo::LOCAL);
                static ImGuizmo::OPERATION gizmoOperation(ImGuizmo::TRANSLATE);
                float* matrix = (float*)glm::value_ptr(UIManager::instance->inspectorui->ObjectSelectToInspector->transform->GetMatrix());

                float* projection = (float*)glm::value_ptr(SceneManager::GetSceneManager()->OpenScene->worldCamera->GetProjectionMatrix());
                float* view = (float*)glm::value_ptr(SceneManager::GetSceneManager()->OpenScene->worldCamera->GetView());
                ImGuizmo::SetRect(p.x, p.y, imageSizeSCENE.x, imageSizeSCENE.y);

                const bool res = ImGuizmo::Manipulate(view, projection, gizmoOperation, gizmoMode, matrix);

                ignoreGui &= !ImGuizmo::IsOver();

                glm::vec3 matrixRotation;
                ImGuizmo::DecomposeMatrixToComponents(
                    matrix,
                    glm::value_ptr(UIManager::instance->inspectorui->ObjectSelectToInspector->transform->Position),
                    glm::value_ptr(matrixRotation),
                    glm::value_ptr(UIManager::instance->inspectorui->ObjectSelectToInspector->transform->Scale)
                );

                //UIManager::instance->inspectorui->ObjectSelectToInspector->transform->Rotation = matrixRotation;

                if (!ImGui::IsMouseDown(1)) {
                    if (InputSystem::InputSystem::GetKey(GLFW_KEY_W)) {
                        gizmoOperation = ImGuizmo::TRANSLATE;
                    }

                    if (InputSystem::InputSystem::GetKey(GLFW_KEY_Q)) {
                        gizmoOperation = ImGuizmo::ROTATE;
                    }

                    if (InputSystem::InputSystem::GetKey(GLFW_KEY_E)) {
                        gizmoOperation = ImGuizmo::SCALE;
                    }
                }
            }

#pragma endregion 
        }

        imagePosition.x += ImGui::GetWindowPos().x;
        imagePosition.y += ImGui::GetWindowPos().y;

        WorldPoint = RenderSystem::RenderSystem::ScreenToViewPort(glm::vec2(imagePosition.x, imagePosition.y), glm::vec2(imageSizeSCENE.x, imageSizeSCENE.y));
        glm::vec2 initialMousePos;  // Posici�n del mouse cuando comenz� el arrastre
        glm::vec2 initialObjectPos; // Posici�n inicial del objeto cuando comenz� el arrastre



        if (ImGui::IsWindowHovered() && !LockWithGizmos && !ImGuizmo::IsOver()) {
            float maxZ = -std::numeric_limits<float>::max();

            CastData* data = new CastData();
            ObjectCaster* caster = new ObjectCaster();

            if (ImGui::IsMouseClicked(0)) {
                if (caster->MouseCast(WorldPoint, data)) {
                    UIManager::instance->inspectorui->SelectEntity(data->object);
                }
                else {
                    UIManager::instance->inspectorui->ObjectSelectToInspector = nullptr;
                }
            }
        }
        ImGui::End();
    ImGui::PopID();
#pragma endregion
}

void SceneUI::ScreenPosToWorldRay(
    int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
    int screenWidth, int screenHeight,  // Window size, in pixels
    glm::mat4 ViewMatrix,               // Camera position and orientation
    glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
    glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
    glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
) {

    // The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
    glm::vec4 lRayStart_NDC(
        ((float)mouseX / (float)screenWidth - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
        ((float)(screenHeight - mouseY) / (float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1] (Invierte la coordenada Y)
        -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
        1.0f
    );
    glm::vec4 lRayEnd_NDC(
        ((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
        ((float)(screenHeight - mouseY) / (float)screenHeight - 0.5f) * 2.0f, // Invierte la coordenada Y
        0.0,
        1.0f
    );



    // The Projection matrix goes from Camera Space to NDC.
    // So inverse(ProjectionMatrix) goes from NDC to Camera Space.
    glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

    // The View Matrix goes from World Space to Camera Space.
    // So inverse(ViewMatrix) goes from Camera Space to World Space.
    glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

    glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
    glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
    glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
    glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


    // Faster way (just one inverse)
    //glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
    //glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
    //glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


    glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
    lRayDir_world = glm::normalize(lRayDir_world);


    out_origin = glm::vec3(lRayStart_world);
    out_direction = glm::normalize(lRayDir_world);
}


bool SceneUI::TestRayOBBIntersection(
    glm::vec3 ray_origin,        // Ray origin, in world space
    glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
    glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
    glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
    glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
    float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
) {

    // Intersection method from Real-Time Rendering and Essential Mathematics for Games

    float tMin = 0.0f;
    float tMax = 100000000.0f;

    glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

    glm::vec3 delta = OBBposition_worldspace - ray_origin;

    // Test intersection with the 2 planes perpendicular to the OBB's X axis
    {
        glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
        float e = glm::dot(xaxis, delta);
        float f = glm::dot(ray_direction, xaxis);

        if (fabs(f) > 0.001f) { // Standard case

            float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
            float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
            // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

            // We want t1 to represent the nearest intersection, 
            // so if it's not the case, invert t1 and t2
            if (t1 > t2) {
                float w = t1; t1 = t2; t2 = w; // swap t1 and t2
            }

            // tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
            if (t2 < tMax)
                tMax = t2;
            // tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
            if (t1 > tMin)
                tMin = t1;

            // And here's the trick :
            // If "far" is closer than "near", then there is NO intersection.
            // See the images in the tutorials for the visual explanation.
            if (tMax < tMin)
                return false;

        }
        else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
            if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
                return false;
        }
    }


    // Test intersection with the 2 planes perpendicular to the OBB's Y axis
    // Exactly the same thing than above.
    {
        glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
        float e = glm::dot(yaxis, delta);
        float f = glm::dot(ray_direction, yaxis);

        if (fabs(f) > 0.001f) {

            float t1 = (e + aabb_min.y) / f;
            float t2 = (e + aabb_max.y) / f;

            if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin)
                tMin = t1;
            if (tMin > tMax)
                return false;

        }
        else {
            if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
                return false;
        }
    }


    // Test intersection with the 2 planes perpendicular to the OBB's Z axis
    // Exactly the same thing than above.
    {
        glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
        float e = glm::dot(zaxis, delta);
        float f = glm::dot(ray_direction, zaxis);

        if (fabs(f) > 0.001f) {

            float t1 = (e + aabb_min.z) / f;
            float t2 = (e + aabb_max.z) / f;

            if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin)
                tMin = t1;
            if (tMin > tMax)
                return false;

        }
        else {
            if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
                return false;
        }
    }

    intersection_distance = tMin;
    return true;

}

glm::vec3 SceneUI::ScreenToWorldRay(float mouseX, float mouseY, int screenWidth, int screenHeight, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    // Normaliza las coordenadas del clic del mouse
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;

    // Combina las matrices de vista y proyecci�n para obtener la matriz de transformaci�n inversa
    glm::mat4 inverseMatrix = glm::inverse(projectionMatrix * viewMatrix);

    // Punto de inicio del rayo en el espacio de proyecci�n
    glm::vec4 rayStart(x, y, -1.0f, 1.0f);

    // Punto de fin del rayo en el espacio de proyecci�n
    glm::vec4 rayEnd(x, y, 1.0f, 1.0f);

    // Desproyecta los puntos en el espacio 3D
    rayStart = inverseMatrix * rayStart;
    rayStart /= rayStart.w;

    rayEnd = inverseMatrix * rayEnd;
    rayEnd /= rayEnd.w;

    // Direcci�n del rayo en el espacio 3D
    glm::vec3 rayDirection = glm::normalize(glm::vec3(rayEnd - rayStart));

    return rayDirection;
}

bool SceneUI::RayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& aabbMin, const glm::vec3& aabbMax) {
    // Calcula los valores t m�nimos y t m�ximos para cada plano del AABB
    float tMin = (aabbMin.x - rayOrigin.x) / rayDirection.x;
    float tMax = (aabbMax.x - rayOrigin.x) / rayDirection.x;

    // Aseg�rate de que tMin sea el valor m�s peque�o y tMax sea el valor m�s grande
    if (tMin > tMax) {
        std::swap(tMin, tMax);
    }

    // Calcula t para el plano Y
    float tYMin = (aabbMin.y - rayOrigin.y) / rayDirection.y;
    float tYMax = (aabbMax.y - rayOrigin.y) / rayDirection.y;

    // Aseg�rate de que tYMin sea el valor m�s peque�o y tYMax sea el valor m�s grande
    if (tYMin > tYMax) {
        std::swap(tYMin, tYMax);
    }

    // Actualiza tMin y tMax para tener en cuenta la intersecci�n en el plano Y
    if ((tMin > tYMax) || (tYMin > tMax)) {
        return false; // No hay intersecci�n con el AABB
    }

    // Actualiza tMin y tMax para tener en cuenta la intersecci�n en el plano Z
    float tZMin = (aabbMin.z - rayOrigin.z) / rayDirection.z;
    float tZMax = (aabbMax.z - rayOrigin.z) / rayDirection.z;

    // Aseg�rate de que tZMin sea el valor m�s peque�o y tZMax sea el valor m�s grande
    if (tZMin > tZMax) {
        std::swap(tZMin, tZMax);
    }

    // Actualiza tMin y tMax para tener en cuenta la intersecci�n en el plano Z
    if ((tMin > tZMax) || (tZMin > tMax)) {
        return false; // No hay intersecci�n con el AABB
    }

    // Si llegamos a este punto, el rayo intersecta con el AABB
    return true;
}


void SceneUI::update() {

}

void SceneUI::lateupdate() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAndDepthTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
}


void SceneUI::fixupdate() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Copiar el contenido del framebuffer en la textura
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, texture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, AppSettings::RenderWidth, AppSettings::RenderHeight, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}


glm::vec2 SceneUI::RotatePoint(const glm::vec2& point, const glm::vec2& center, float angle) {
    glm::vec2 rotatedPoint;
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    rotatedPoint.x = center.x + (point.x - center.x) * cosTheta - (point.y - center.y) * sinTheta;
    rotatedPoint.y = center.y + (point.x - center.x) * sinTheta + (point.y - center.y) * cosTheta;

    return rotatedPoint;
}