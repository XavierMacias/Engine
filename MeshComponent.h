#ifndef __mesh_component__
#define __mesh_component__

#include "Component.h"
#include "Globals.h"

class MeshComponent : public Component
{
public:
	enum Meshes
	{
		AmongUs,
		Fox,
		BakerHouse,
		Robot
	};

private:
	const char* meshName;
	UID mesh = 0;

public:
	MeshComponent(GameObject* gameObject);
	~MeshComponent();

	void SetMesh(unsigned int mesh);
	const char* GetMeshName() const { return meshName; }
	void SelectMesh(MeshComponent::Meshes mesh);

};

#endif __mesh_component__