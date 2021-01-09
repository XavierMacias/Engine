#include "ModuleScene.h"

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() {}

GameObject* ModuleScene::CreateGameObject(GameObject* parent, const char* name, bool root)
{
	GameObject* gameObject = new GameObject(parent, name, root);
	game_objects.push_back(gameObject);
	if (!gameObject->GetRoot()) {
		FindGameObject(parent)->children.push_back(gameObject);
	}
	
	return gameObject;
}

GameObject* ModuleScene::FindGameObject(GameObject* p) {
	GameObject* go = NULL;
	for (int i = 0; i < game_objects.size(); ++i) {
		if (game_objects[i] == p) {
			go = game_objects[i];
		}
	}

	return go;
}

void ModuleScene::RemoveGameObject(GameObject* current) {

	if (current->children.size() > 0) {
		for (int i = 0; i < current->children.size(); ++i) {
			RemoveGameObject(current->children[i]);
		}
	}

	if (current->GetParent() != NULL) {
		current->GetParent()->children.erase(std::remove(current->GetParent()->children.begin(), current->GetParent()->children.end(), current), current->GetParent()->children.end());
	}
	game_objects.erase(std::remove(game_objects.begin(), game_objects.end(), current), game_objects.end());
	delete current;
	
}

void ModuleScene::SetNewParent(GameObject* newparent, GameObject* newchild) {


	newchild->GetParent()->children.erase(std::remove(newchild->GetParent()->children.begin(), newchild->GetParent()->children.end(), newchild), newchild->GetParent()->children.end());
	newchild->setParent(newparent);
	newparent->children.push_back(newchild);
}
