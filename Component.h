#ifndef __component__
#define __component__

#include <string>

enum ComponentType
{
	TRANSFORM_COMPONENT,
	MESH_COMPONENT,
	MATERIAL_COMPONENT
};

class Component
{
public:
	/*Component();
	~Component();*/

	/*virtual void Enable();
	virtual void Disable();
	virtual void Update();*/

private:
	std::string name;
	
};


#endif __component__