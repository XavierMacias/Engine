#include "CameraComponent.h"

CameraComponent::CameraComponent(GameObject* gameObject, std::string name) : Component(gameObject, ComponentType::CAMERA_COMPONENT, name) {
}

CameraComponent::~CameraComponent() {}
