#ifndef __mesh_component__
#define __mesh_component__

#include "Mesh.h"
#include "Component.h"
#include "Globals.h"
#include <string>

class MeshComponent : public Component
{
private:
	//char* meshName = "None";
	Mesh meshResource;

public:
	MeshComponent(GameObject* gameObject, std::string name);
	~MeshComponent();

	//char* GetMeshName() { return meshName; }
	void SetMesh(Mesh mesh) { meshResource = mesh; }
	Mesh GetMesh() { return meshResource; }

};

#endif __mesh_component__