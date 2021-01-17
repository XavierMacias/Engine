#ifndef __component__
#define __component__

#include "Application.h"
#include <string>

class GameObject;

class Component
{
public:
	enum ComponentType
	{
		MESH_COMPONENT,
		MATERIAL_COMPONENT
	};

private:
	std::string name;
	bool active;
	GameObject* owner;
	ComponentType type;
	int uid;


public:
	Component(GameObject* gameObject, ComponentType type, std::string n);
	~Component();

	void Enable(bool active);

	virtual void Init() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	void Remove();

	Component::ComponentType GetType() const;
	const char* GetTypeStr() const;
	int GetUID() { return uid; }
	std::string GetName() { return name; }
	GameObject* GetOwner() { return owner; }

};


#endif __component__