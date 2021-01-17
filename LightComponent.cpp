#include "LightComponent.h"

LightComponent::LightComponent(GameObject* gameObject, std::string name) : Component(gameObject, ComponentType::LIGHT_COMPONENT, name) {
}

LightComponent::~LightComponent() {}
