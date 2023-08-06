#include "../UIDrawer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

unsigned int framebuffer;
unsigned int texture;
int textureWidth, textureHeight;
ImVec2 imagePosition;
DrawBox* box = new DrawBox();

class SceneUI : public UIDrawer {

public:
    unsigned int framebuffer;
    unsigned int texture;
    ImVec2 imageSizeSCENE;
    double textureMousePosX = 0;
    double textureMousePosY = 0;

    void start() override {
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, AppSettings::instance->ScreenWidth, AppSettings::instance->ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    void draw() override {

        ImVec2 windowSize = ImVec2(AppSettings::ScreenWidth, AppSettings::ScreenHeight);

        // Dibujamos la imagen en la ventana de ImGui
        ImGui::SetNextWindowSize(windowSize);
        ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        //RenderSizeWindow = ImGui::GetWindowSize();

        // Obtenemos el tama�o de la ventana de ImGui despu�s de que se apliquen las restricciones de tama�o
        ImVec2 actualWindowSize = ImGui::GetWindowSize();
        float scaleFactor = std::min(actualWindowSize.x / windowSize.x, actualWindowSize.y / windowSize.y);
        imageSizeSCENE = ImVec2(AppSettings::ScreenWidth * scaleFactor, AppSettings::ScreenHeight * scaleFactor);


        // Calculamos la posici�n para centrar la imagen en la ventana
        imagePosition = ImVec2((actualWindowSize.x - imageSizeSCENE.x) * 0.5f, (actualWindowSize.y - imageSizeSCENE.y) * 0.5f);

        ImGui::SetCursorPos(imagePosition);

        int WindowXSize = imageSizeSCENE.x;
        int WindowYSize = imageSizeSCENE.y;
        imageSizeSCENE.x = WindowXSize;
        imageSizeSCENE.y = WindowYSize;

        // Invertimos las coordenadas de textura en el eje Y antes de mostrar la imagen
        ImGui::Image((void*)(intptr_t)texture, ImVec2(WindowXSize, WindowYSize), ImVec2(1, 1), ImVec2(0, 0));

        //std::cout << "Nuevo tama�o de la textura: " << imageSizeSCENE.x << "x" << imageSizeSCENE.y << std::endl;

        // Despu�s de obtener el nuevo tama�o de la textura, puedes usarlo como lo necesites

        imagePosition.x += ImGui::GetWindowPos().x;
        imagePosition.y += ImGui::GetWindowPos().y;

        double x, y;
        glfwGetCursorPos(StartEngineGraphics::window, &x, &y);

        double windowMousePosX = x - imagePosition.x;
        double windowMousePosY = y - imagePosition.y;

        double NormalMousePosX = windowMousePosX / imageSizeSCENE.x;
        double NormalMousePosY = -windowMousePosY / imageSizeSCENE.y;

        // Calcular el centro de la textura en coordenadas de la ventana
        double centeredMousePosX = (NormalMousePosX * 2.0f - 1.0f) * AppSettings::ScreenWidth / 2;
        double centeredMousePosY = (NormalMousePosY * 2.0f + 1.0f) * AppSettings::ScreenHeight / 2;

        Camera* cam = SceneManager::GetSceneManager()->OpenScene->worldCamera;

        double WorldPointX = centeredMousePosX + cam->cameraPosition.x;
        double WorldPointY = centeredMousePosY - cam->cameraPosition.y;


        for (Entity* objD : SceneManager::GetSceneManager()->OpenScene->objectsInScene) {
            glm::vec3& obj = objD->getComponent<SpriteComponent>().cubePosition;

            // Convertir las coordenadas del objeto al espacio de la c�mara
            float objWidth = 25 * SceneManager::GetSceneManager()->OpenScene->worldCamera->zoom;
            float objHeight = 25 * SceneManager::GetSceneManager()->OpenScene->worldCamera->zoom;

            // Ajustar las coordenadas del objeto para que est�n centradas en el espacio de la c�maraf
            float objX = obj.x - objWidth * 0.5f;
            float objY = -obj.y - objHeight * 0.5f;

            if (WorldPointX >= objX && WorldPointX <= objX + objWidth &&
                WorldPointY >= objY && WorldPointY <= objY + objHeight) {
                // Hacer clic en el objeto (realizar la acci�n deseada)
                std::cout << "Objeto cliqueado: " << objD->ObjectTag << std::endl;
                // Agregar aqu� la l�gica para la acci�n deseada para el objeto clickeado
                break; // Si solo quieres detectar un objeto clickeado, puedes agregar break aqu�
            }
        }




        ImGui::Begin("Picking Position");
        ImGui::Text("Mouse Window X: %f", windowMousePosX);
        ImGui::Text("Mouse Window Y: %f", windowMousePosY);
        ImGui::Spacing();
        ImGui::Text("Mouse Scene X: %f", WorldPointX);
        ImGui::Text("Mouse Scene Y: %f", WorldPointY);
        ImGui::Spacing();
        ImGui::Text("Render Size X: %f", imageSizeSCENE.x);
        ImGui::Text("Render Size Y: %f", imageSizeSCENE.y);
        ImGui::End();

        ImGui::Begin("OBJECTS IN SCENE");


        /*
        ImGui::Text("Object tag: %f", objD->ObjectTag.c_str());
        ImGui::Text("Pos x: : %f", objX);
        ImGui::Text("Pos y: : %f", objY);
        */
        ImGui::End();

        //ImGui::Begin("OBJECTS IN SCENE");
        //ImGui::Text("Object tag: %f", objD->ObjectTag.c_str());
        //ImGui::Text("Pos x: : %f", objX);
        //ImGui::Text("Pos y: : %f", objY);
        //
        //ImGui::End();


        ImGui::End();


        ImGui::Begin("ASSETS");
        ImGui::Button("LAKAKA");
        ImGui::End();
    }

    void update() {

    }

    void lateupdate() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }


    void fixupdate() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Copiar el contenido del framebuffer en la textura
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindTexture(GL_TEXTURE_2D, texture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, AppSettings::instance->ScreenWidth, AppSettings::instance->ScreenHeight, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }
};




/* PICKING WORKING WITHOUT CAMERA POSITION


 double x, y;
        glfwGetCursorPos(StartEngineGraphics::window, &x, &y);

        double windowMousePosX = x - imagePosition.x;
        double windowMousePosY = y - imagePosition.y;

        // Obtener el ancho y alto de la imagen o textura
        double imageWidth = imageSizeSCENE.x; // Ancho de la imagen
        double imageHeight = imageSizeSCENE.y; // Alto de la imagen


        // Ajustar la posici�n para que el centro sea el punto de origen
        double centeredMousePosX = windowMousePosX - (imageWidth * 0.5);
        double centeredMousePosY = windowMousePosY - (imageHeight * 0.5);

        // Obtener el tama�o actual de la ventana
        ImVec2 actualWindowSize2 = ImGui::GetWindowSize();

        // Obtener el factor de escala para convertir las coordenadas del mouse a las coordenadas en el espacio de la textura
        float scaleFactor2 = std::min(actualWindowSize2.x / windowSize.x, actualWindowSize2.y / windowSize.y);
        textureMousePosX = centeredMousePosX / scaleFactor2;
        textureMousePosY = centeredMousePosY / scaleFactor2;



        //if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        //
        //}

        ImGui::Begin("Picking Position");
        ImGui::Text("Mouse X: %f", textureMousePosX);
        ImGui::Text("Mouse Y: %f", textureMousePosY);
        ImGui::End();

        ImGui::Begin("OBJECTS IN SCENE");

        for (Entity* objD : SceneManager::GetSceneManager()->OpenScene->objectsInScene) {
            glm::vec3& obj = objD->getComponent<SpriteComponent>().cubePosition;

            // Convertir las coordenadas del objeto al espacio de la c�mara
            glm::vec3 objPosCamSpace = obj - SceneManager::GetSceneManager()->OpenScene->worldCamera->cameraPosition;

            float objWidth = 25;
            float objHeight = 25;

            // Ajustar las coordenadas del objeto para que est�n centradas en el espacio de la c�mara
            float objX = objPosCamSpace.x - objWidth * 0.5f;
            float objY = objPosCamSpace.y - objHeight * 0.5f;

            ImGui::Text("Object tag: %f", objD->ObjectTag.c_str());
            ImGui::Text("Pos x: : %f", objX);
            ImGui::Text("Pos y: : %f", objY);

            if (textureMousePosX >= objX && textureMousePosX <= objX + objWidth &&
                textureMousePosY >= objY && textureMousePosY <= objY + objHeight) {
                // Hacer clic en el objeto (realizar la acci�n deseada)
                std::cout << "Objeto cliqueado: " << objD->ObjectTag << std::endl;
                // Agregar aqu� la l�gica para la acci�n deseada para el objeto clickeado
                break; // Si solo quieres detectar un objeto clickeado, puedes agregar break aqu�
            }
        }
        ImGui::End();


if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {

    }

*/