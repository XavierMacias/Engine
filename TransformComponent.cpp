#include "TransformComponent.h"

TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject, ComponentType::TRANSFORM_COMPONENT) {}

TransformComponent::~TransformComponent() {}

//void TransformComponent::Update()
//{
//
//}

void TransformComponent::setPosition(const float3& pos)
{
	
}

void TransformComponent::setRotation(const float3& rot)
{

}

void TransformComponent::setScale(const float3& sc)
{

}

