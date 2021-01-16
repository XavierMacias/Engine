#include "MeshComponent.h"
#include "Mesh.h"

MeshComponent::MeshComponent(GameObject* gameObject) : Component(gameObject, ComponentType::MESH_COMPONENT) {}

MeshComponent::~MeshComponent() {}



void MeshComponent::SelectMesh(MeshComponent::Meshes meshselected)
{	
	switch (meshselected)
	{
	case MeshComponent::AmongUs:
		meshName = "AmongUs";
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
		break;
	}
}



