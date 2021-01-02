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
	GameObject* CreateGameObject(GameObject* parent, const char* name);

};

