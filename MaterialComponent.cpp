#include "MaterialComponent.h"

MaterialComponent::MaterialComponent(GameObject* gameObject, std::string name) : Component(gameObject, ComponentType::MATERIAL_COMPONENT, name) {}

MaterialComponent::~MaterialComponent() {}

