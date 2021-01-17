#include "Component.h"

Component::Component(GameObject* ow, ComponentType t, std::string n) {
	uid = App->random->Int();
	name = n;
	owner = ow;
	type = t;
}

Component::~Component() {}

void Component::Enable(bool active)
{
	if (this->active != active)
	{
		this->active = active;		
	}
}

Component::ComponentType Component::GetType() const 
{ 
	return type; 
}

const char* Component::GetTypeStr() const
{
	static const char* names[] = 
	{
		"Mesh",
		"Material",
		"Camera",
	 };

	return names[type];
}

