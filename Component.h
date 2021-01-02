#ifndef __component__
#define __component__


#include <string>

class GameObject;

class Component
{
private:
	std::string name;
	bool active;
	GameObject* owner;	

public:
	enum ComponentType
	{
		TRANSFORM_COMPONENT,
		MESH_COMPONENT,
		MATERIAL_COMPONENT
	};

public:
	Component(GameObject* gameObject, ComponentType type);
	~Component();

	void Enable(bool active);	

	virtual void Init() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	void Remove();
};


#endif __component__