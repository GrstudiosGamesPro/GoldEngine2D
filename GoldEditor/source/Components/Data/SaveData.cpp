#include "SaveData.h"
#include <iostream>
#include "../Components/ECS/ComponentFactory.h"
#include "../../Editor/GoldEditor.h"

using namespace std;
using namespace nlohmann;

SaveData::SaveData() {

}


void SaveData::saveScene() {
    json GameDataSettings;
    GameDataSettings["StartScene"] = "SampleScene.scene";

    json objectData;
    json objects = json::array();


    if (SceneManager::GetSceneManager() != nullptr) {
        for (int i = 0; i < SceneManager::GetSceneManager()->OpenScene->objectsInScene.size(); i++) {
            json objectVal;
            Entity* getOBJ = SceneManager::GetSceneManager()->OpenScene->objectsInScene[i];

            objectVal["name"] = getOBJ->ObjectName;
            objectVal["tag"] = getOBJ->ObjectTag;
            objectVal["transform"] = getOBJ->transform->serialize();
            
            if (getOBJ->parent != nullptr) {
                objectVal["parentID"] = getOBJ->parent->objectID;
            }

            json components;

            std::vector<Component*> cmpms = getOBJ->getComponents<Component>();

            for (int e = 0; e < cmpms.size(); e++) {
                json cpmOBJ;
                Component* selectComponent = cmpms[e];
                string typeg = typeid(*selectComponent).name();

                std::string newName = typeg;
                size_t pos = newName.find("class ");
                if (pos != std::string::npos) {
                    newName.replace(pos, 6, "");
                }

                //std::cout << "EL TIPO DE COMPONENTE ES " << newName << std::endl;
                cpmOBJ["componenttype"] = newName;
                cpmOBJ["componentdata"] = json::parse (selectComponent->serialize());

                components.push_back (cpmOBJ);
            }

            objectVal["components"] = components;

            objects.push_back (objectVal);
        }
    }

    objectData["objects"] = objects;
    GLD::SaveSystem::save(GoldEditor::editor->ProjectPath + "/assets", "GameSettings.data", GameDataSettings.dump(4));
    GLD::SaveSystem::save(GoldEditor::editor->ProjectPath + "/assets", SceneManager::GetOpenSceneName()->c_str(), objectData.dump(4));
}




void SaveData::loadScene() {
    string LoadedGameSettings = GLD::SaveSystem::load(GoldEditor::editor->ProjectPath + "/assets/", "GameSettings.data");
    json dataJson = json::parse(LoadedGameSettings);

    std::cout << "Game Settings Loaded" << (string)dataJson["StartScene"] << std::endl;

    SceneManager::LoadScene(GoldEditor::editor->ProjectPath + "/assets/", (string)dataJson["StartScene"]);
    //if (GLD::SaveSystem::load(GoldEditor::editor->ProjectPath + "/assets", "samplescene.scene").empty()) {
    //    std::cout << "ARCHIVO VACIO O NO EXISTENTE" << std::endl;
    //    return;
    //}
    //string loadJsonDAta = GLD::SaveSystem::load(GoldEditor::editor->ProjectPath + "/assets", "samplescene.scene");
    //json loadJson = json::parse (loadJsonDAta);

    //std::cout << "OBJETOS CARGADOS: " << loadJson["objects"].size();
    //for (int i = 0; i < loadJson["objects"].size(); i++) {
    //    Entity* newEntity = SceneManager::GetSceneManager()->NewEntity();
    //    json getEntityData = loadJson["objects"][i];
    //    newEntity->ObjectName = getEntityData["name"];
    //    newEntity->ObjectTag = getEntityData["tag"];
    //    newEntity->transform->deserialize(getEntityData["transform"], "");


    //    std::vector<Component*> cmpms = newEntity->getComponents<Component>();

    //    for (int e = 0; e < cmpms.size(); e++) {
    //        json cpmOBJ;
    //        Component* selectComponent = cmpms[e];
    //        string typeg = typeid(*selectComponent).name();

    //        std::string newName = typeg;
    //        size_t pos = newName.find("class ");
    //        if (pos != std::string::npos) {
    //            newName.replace(pos, 6, "");
    //        }

    //        for (int j = 0; j < getEntityData["components"].size(); j++) {
    //            json getComponent = getEntityData["components"][j];


    //            //if (getComponent["componenttype"] == newName) {
    //            //    selectComponent->deserialize (getComponent["componentdata"].dump());
    //            //}

    //            ComponentFactory::loadComponent(getComponent["componenttype"].dump(), newEntity, getComponent["componentdata"].dump(), "");
    //        }
    //    }
    //}
}