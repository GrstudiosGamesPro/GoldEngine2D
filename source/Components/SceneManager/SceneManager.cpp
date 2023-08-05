#include "SceneManager.h"
#include <iostream>;

using namespace std;


SceneManager* SceneManager::instance = nullptr;

void SceneManager::create()
{
	if (SceneManager::instance) throw std::exception("SceneManager already created.");
	SceneManager::instance = new SceneManager();
	std::cout << "SceneManager Created" << endl;
}



void SceneManager::release()
{
	if (!SceneManager::instance) return;
	delete SceneManager::instance;
}


SceneManager* SceneManager::GetSceneManager() {
	return instance;
}

Entity* SceneManager::NewEntity() {
	Entity* newObj = new Entity();
	
	OpenScene->objectsInScene.push_back(newObj);
	return newObj;
}


Entity* SceneManager::GetObjectByID (int id) {
	// Aseg�rate de que OpenScene y objectsInScene no sean nulos
	if (OpenScene && OpenScene->objectsInScene.size() > 0) {
		// Busca el objeto con el ID deseado en el vector de objetos de la escena
		for (Entity* obj : OpenScene->objectsInScene) {
			if (obj->objectID == id) {
				return obj; // Se encontr� el objeto con el ID especificado
			}
		}
	}
	return nullptr; // No se encontr� el objeto con el ID especificado o el vector estaba vac�o
}