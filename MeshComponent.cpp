#include "MeshComponent.h"

MeshComponent::MeshComponent(GameObject* gameObject) : Component(gameObject, ComponentType::MESH_COMPONENT) {}

MeshComponent::~MeshComponent() {}