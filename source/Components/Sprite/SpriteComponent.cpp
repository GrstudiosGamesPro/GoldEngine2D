#include "SpriteComponent.h"
#define STB_IMAGE_IMPLEMENTATIONTEXTURE
#include <stb_image.h>
#include "nlohmann/json.hpp"
#include "../SaveSystem/CheckVar.h"

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

const char* vertexShaderSource2 = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    out vec2 TexCoord;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource2 = R"(
    #version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;

    uniform sampler2D textureSampler;

    void main() {
        FragColor = texture(textureSampler, TexCoord);
    }
)";

void SpriteComponent::compileShaders() {
    ourShader = new Shader(VertexPath.c_str(), FragmentPath.c_str());
    ourmodel = new GLD::Model("F:\\VISUAL STUDIO\\GoldEngine2D\\GoldEditor\\def/models/Plane.fbx");
}

void SpriteComponent::start()  {
    compileShaders();
    /*vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Configuraciones de la textura
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glGenerateMipmap  (GL_TEXTURE_2D);
    //glTexParameteri   (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameterf   (GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LoadTexture ();
}

void SpriteComponent::LoadTexture () {
    std::cout << "La nueva ruta de la textura es " << TexturePath << std::endl;
    int width, height, nrChannels;

    unsigned char* data = stbi_load(TexturePath.c_str(), &width, &height, &nrChannels, 0);
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
    glUseProgram(ourShader->ID);
    glUniformMatrix4fv(glGetUniformLocation(ourShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(SceneManager::GetSceneManager()->OpenScene->worldCamera->GetProjectionMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(ourShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(SceneManager::GetSceneManager()->OpenScene->worldCamera->GetView()));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(ourShader->ID, "textureSampler"), 0);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(ObjectPosition.x, ObjectPosition.y, ObjectPosition.z));
    model = glm::rotate(model, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotaci�n en el eje Z
    model = glm::rotate(model, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotaci�n en el eje Y
    model = glm::rotate(model, glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotaci�n en el eje X
    model = glm::scale(model, glm::vec3(Scale.x * GlobalScale, Scale.y * GlobalScale, 25));

    //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    //std::cout << "ROTATION X: " << rotationAngleX << std::endl;

    ////glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(ObjectPosition.x, -ObjectPosition.y, ObjectPosition.z));
    ////model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotaci�n en el eje Z
    ////model = glm::scale(model, glm::vec3(Scale.x * GlobalScale, Scale.y * GlobalScale, 25));
    ////glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    //glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

    
    ourShader->setMat4("model", model);
    ourmodel->Draw(*ourShader);
}


void SpriteComponent::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram);
}



std::string SpriteComponent::serialize() {
    json componentData;
    componentData["posx"] = ObjectPosition.x;
    componentData["posy"] = ObjectPosition.y;
    componentData["posz"] = ObjectPosition.z;
    componentData["scalex"] = Scale.x;
    componentData["scaley"] = Scale.y;
    componentData["scaleglobal"] = GlobalScale;
    componentData["rotation"] = rotationAngle;
    componentData["texturepath"] = TexturePath;
    componentData["vertexpath"] = TexturePath;
    componentData["fragmentpath"] = TexturePath;

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

    if (CheckVar::Has(componentData, "scaleglobal"))
    GlobalScale = componentData["scaleglobal"];

    if (CheckVar::Has(componentData, "rotation"))
    rotationAngle = (float)componentData["rotation"];

    if (CheckVar::Has(componentData, "texturepath"))
    TexturePath = path + (string)componentData["texturepath"];


    if (CheckVar::Has(componentData, "vertexpath"))
        VertexPath = path + (string)componentData["vertexpath"];


    if (CheckVar::Has(componentData, "fragmentpath"))
        FragmentPath = path + (string)componentData["fragmentpath"];

    LoadTexture();
}


glm::mat4 SpriteComponent::GetMatrix() {
    glm::mat4 matrix;

    matrix = glm::translate  (glm::mat4 (1.0f), glm::vec3 (ObjectPosition.x, ObjectPosition.y, ObjectPosition.z));
    matrix *= glm::mat4_cast (glm::quat (1, rotationAngleX, rotationAngleY, rotationAngleZ));
    matrix = glm::scale (matrix, Scale);

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