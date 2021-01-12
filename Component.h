#ifndef __component__
#define __component__


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

public:
	Component(GameObject* gameObject, ComponentType type);
	~Component();

	void Enable(bool active);	

	virtual void Init() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	void Remove();

	Component::ComponentType GetType() const;
	const char* GetTypeStr() const;


};


#endif __component__