#pragma once
#include "Application.h"
#include "Module.h"
#include "GameObject.h"
#include "ModuleModel.h"
#include <vector>

class ModuleScene : public Module
{
private:
	std::vector <GameObject*> game_objects;
	GameObject* root = nullptr;

public:
	ModuleScene();
	~ModuleScene();
	bool Start();

	GameObject* CreateGameObject(GameObject* parent, const char* name, bool root);
	std::vector<GameObject*> GetGameObjects() { return game_objects; }
	GameObject* FindGameObject(GameObject* parent);
	void RemoveGameObject(GameObject* current);
	void SetNewParent(GameObject* newparent, GameObject* newchild);
	GameObject* GetRoot() { return root; }
	void AddObject(const char* path);
	std::string GetFilename(const std::string s);

};

