#include "ModuleScene.h"

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() {}

GameObject* ModuleScene::CreateGameObject(GameObject* parent, const char* name)
{
	GameObject* gameObject = new GameObject(parent, name);

	return gameObject;
}
