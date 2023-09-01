#include "SpriteComponent.h"
#define STB_IMAGE_IMPLEMENTATIONTEXTURE
#include <stb_image.h>
#include "nlohmann/json.hpp"
#include "../SaveSystem/CheckVar.h"
#include "../FileSystem/FileSystem.h"

using namespace nlohmann;

float vertices2[] = {
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f
};

unsigned int indices2[] = {
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4,
    0, 4, 5,
    5, 1, 0,
    1, 5, 6,
    6, 2, 1,
    2, 6, 7,
    7, 3, 2,
    3, 7, 4,
    4, 0, 3
};

void SpriteComponent::start()  {
    compileShaders();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Configuraciones de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LoadTexture ();
}

void SpriteComponent::LoadTexture () {
    std::cout << "La nueva ruta de la textura es " << TexturePath << std::endl;
    int width, height, nrChannels;
    string newPath = FileSystem::GetAsset (TexturePath);
    std::cout << "NUEVA RUTA:" << newPath << std::endl;

    unsigned char* data = stbi_load(newPath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

void SpriteComponent::onupdate() {
    
}

void SpriteComponent::draw() {
    glUseProgram(ourShader->ID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(ourShader->ID, "texture_diffuse1"), 0);


    // Aplicar rotación utilizando glm::rotate
    model = glm::mat4(1.0f);

    // Aplicar traslación

    model = glm::translate(model, glm::vec3(ObjectPosition.x, ObjectPosition.y, ObjectPosition.z));
    glm::quat rotationZ = glm::angleAxis(glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::quat rotationY = glm::angleAxis(glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rotationX = glm::angleAxis(glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));

    rotation = rotationZ * rotationY * rotationX;

    //glm::mat4 rotationMatrix = glm::mat4_cast(rotation);


    model *= glm::mat4_cast(rotation);
    model = glm::scale(model, glm::vec3(Scale.x * GlobalScale, Scale.y * GlobalScale, Scale.z * GlobalScale));

    ourShader->use();
    ourShader->setMat4("view", SceneManager::GetSceneManager()->OpenScene->worldCamera->GetView());
    ourShader->setMat4("projection", SceneManager::GetSceneManager()->OpenScene->worldCamera->GetProjectionMatrix());
    ourShader->setMat4("model", model);  // Aplicar la matriz de modelo
    ourmodel->Draw(*ourShader);
}

void SpriteComponent::PreRender() {
 
}

void SpriteComponent::PostRender() {

}


void SpriteComponent::clean() {

}



std::string SpriteComponent::serialize() {
    json componentData;
    componentData["posx"] = ObjectPosition.x;
    componentData["posy"] = ObjectPosition.y;
    componentData["posz"] = ObjectPosition.z;
    componentData["scalex"] = Scale.x;
    componentData["scaley"] = Scale.y;
    componentData["scalez"] = Scale.z;
    componentData["scaleglobal"] = GlobalScale;
    componentData["rotationx"] = rotationAngleX;
    componentData["rotationy"] = rotationAngleY;
    componentData["rotationz"] = rotationAngleZ;
    componentData["texturepath"] = TexturePath;
    componentData["vertexpath"] = VertexPath;
    componentData["fragmentpath"] = FragmentPath;

    return componentData.dump();
}

void SpriteComponent::deserialize (std::string g, std::string path) {
    json componentData = json::parse(g);


    if (CheckVar::Has(componentData, "posx"))
    ObjectPosition.x = (float)componentData["posx"];

    if (CheckVar::Has (componentData, "posy"))
    ObjectPosition.y = componentData["posy"];

    if (CheckVar::Has(componentData, "posz"))
    ObjectPosition.z = componentData["posz"];

    if (CheckVar::Has(componentData, "scalex"))
    Scale.x = componentData["scalex"];

    if (CheckVar::Has(componentData, "scaley"))
    Scale.y = componentData["scaley"];

    if (CheckVar::Has(componentData, "scalez"))
    Scale.z = componentData["scalez"];

    if (CheckVar::Has(componentData, "scaleglobal"))
    GlobalScale = componentData["scaleglobal"];

    if (CheckVar::Has(componentData, "rotationx"))
    rotationAngleX = (float)componentData["rotationx"];

    if (CheckVar::Has(componentData, "rotationy"))
    rotationAngleY = (float)componentData["rotationy"];

    if (CheckVar::Has(componentData, "rotationz"))
    rotationAngleZ = (float)componentData["rotationz"];

    if (CheckVar::Has(componentData, "texturepath")) {
        string newPath = (string)componentData["texturepath"];
        TexturePath = newPath;
    }


    if (CheckVar::Has(componentData, "vertexpath"))
    VertexPath = (string)componentData["vertexpath"];


    if (CheckVar::Has(componentData, "fragmentpath"))
    FragmentPath = (string)componentData["fragmentpath"];

    LoadTexture();
}

glm::mat4 SpriteComponent::GetMatrix() {
    glm::mat4 matrix;

    matrix = glm::translate(glm::mat4(1.0f), glm::vec3(ObjectPosition.x, ObjectPosition.y, ObjectPosition.z));
    matrix *= glm::mat4_cast(glm::quat(1, 0, 0, 0));
    matrix = glm::scale(matrix, Scale);

    return matrix;
}



float* SpriteComponent::getVertices() {
    return vertices2;
}

std::size_t SpriteComponent::getIndicesSize() {
    return sizeof(indices2);
}



GLuint* SpriteComponent::getIndices() {
    return indices2;
}

void SpriteComponent::compileShaders() {
    string newPathVertex = FileSystem::GetAsset(VertexPath);
    string newPathFrag = FileSystem::GetAsset(FragmentPath);

    ourShader = new Shader(newPathVertex.c_str(), newPathFrag.c_str());
    ourmodel = new GLD::Model("E:\\VISUAL STUDIO\\GoldEngine2D\\GoldEditor\\def/models/Terrain.fbx");
}