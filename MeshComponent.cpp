#include "MeshComponent.h"
#include "Mesh.h"

MeshComponent::MeshComponent(GameObject* gameObject, std::string name) : Component(gameObject, ComponentType::MESH_COMPONENT, name) {
}

MeshComponent::~MeshComponent() {}



