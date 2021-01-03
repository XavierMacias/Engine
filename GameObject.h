#ifndef __Game_Object__
#define __Game_Object__

#include <string>

#include "Component.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"

#include <vector>

class GameObject
{
public:
	~GameObject();
	GameObject();

	void Update();
	Component* CreateComponent(ComponentType type);	
	
private:
	bool active;
	std::string name;

	TransformComponent* transform_;	
	MeshComponent* mesh_;
	MaterialComponent* material_;

	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent;
};

#endif __Game_Object__
