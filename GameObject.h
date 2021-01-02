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

public:
	bool active;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
	
public:
	GameObject(GameObject* parent, const char* name);
	~GameObject();	
	
	void Update();
	Component* CreateComponent(Component::ComponentType type);		

	void setParent(GameObject* parent);
};

#endif __Game_Object__
