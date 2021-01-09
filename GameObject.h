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

	TransformComponent* transform_;	
	MeshComponent* mesh_;
	MaterialComponent* material_;

	void setParent(GameObject* parent);
};

#endif __Game_Object__
