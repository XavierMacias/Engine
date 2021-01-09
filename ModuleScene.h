#pragma once
#include "Module.h"
#include "GameObject.h"
#include <vector>

class ModuleScene : public Module
{
private:
	std::vector <GameObject*> game_objects;

public:
	ModuleScene();
	~ModuleScene();
	GameObject* CreateGameObject(GameObject* parent, const char* name, bool root);
	std::vector<GameObject*> GetGameObjects() { return game_objects; }
	GameObject* FindGameObject(GameObject* parent);
	void RemoveGameObject(GameObject* current);
	void SetNewParent(GameObject* newparent, GameObject* newchild);

};

