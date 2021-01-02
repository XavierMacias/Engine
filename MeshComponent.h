#ifndef __mesh_component__
#define __mesh_component__

#include "Component.h"
#include <string>

class MeshComponent : public Component
{
private:
	std::string meshName;
	
public:
	MeshComponent(GameObject* gameObject);
	~MeshComponent();

	void setMesh(unsigned int mesh);
	void getMesh();
};

#endif __mesh_component__