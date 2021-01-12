#include "MeshComponent.h"
#include "Mesh.h"

MeshComponent::MeshComponent(GameObject* gameObject) : Component(gameObject, ComponentType::MESH_COMPONENT) {}

MeshComponent::~MeshComponent() {}

void MeshComponent::SetMesh(unsigned int mesh)
{
	
}

void MeshComponent::SelectMesh(MeshComponent::Meshes meshselected)
{
	const Mesh mesh;
	switch (meshselected)
	{
	case MeshComponent::AmongUs:
		meshName = "Among Us";
		//Load the mesh
		break;
	case MeshComponent::Fox:
		meshName = "Fox";
		break;
	case MeshComponent::BakerHouse:
		meshName = "BakerHouse";
		break;
	case MeshComponent::Robot:
		meshName = "Robot";
		break;
	default:
		meshName = "None";
		break;
	}
}



