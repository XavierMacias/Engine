#ifndef __mesh_component__
#define __mesh_component__

#include "Mesh.h"
#include "Component.h"
#include "Globals.h"

class MeshComponent : public Component
{
public:

	//Swap when the file importer be ready
	enum Meshes
	{
		AmongUs,
		Fox,
		BakerHouse,
		Robot
	};

private:
	char* meshName = "None";
	UID mesh = 0;
	Mesh meshResource;

public:
	MeshComponent(GameObject* gameObject);
	~MeshComponent();

	char* GetMeshName() { return meshName; }
	void SelectMesh(MeshComponent::Meshes mesh);

};

#endif __mesh_component__