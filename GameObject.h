#ifndef __Game_Object__
#define __Game_Object__

#include <string>
#include <vector>
#include<list>

#include "Component.h"

class GameObject
{
	friend class Component;

private:
	GameObject* parent;
	unsigned int uid;

public:
	bool active;
	const char* name;
	bool isRoot;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

public:
	GameObject(GameObject* parent, const char* name, bool isRoot);
	~GameObject();

	void Update();
	Component* CreateComponent(Component::ComponentType type);
	GameObject* GetParent() { return parent; }
	void setParent(GameObject* parent);
	bool GetRoot() { return isRoot; }
	int GetUid() { return uid; }
	void SetName(char* n) { name = n; }

};

#endif __Game_Object__
