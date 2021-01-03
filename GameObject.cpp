#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"

GameObject::GameObject(GameObject* parent, const char* name) : name(name)
{
	
}

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		delete* it;
	}
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		delete* it;
	}
}

void GameObject::Update()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->Update();
}

Component* GameObject::CreateComponent(Component::ComponentType type)
{
	Component* component = nullptr;

	switch (type)
	{
	case Component::ComponentType::TRANSFORM_COMPONENT:
	{
		component = new TransformComponent(this);
		break;
	}
	case Component::ComponentType::MESH_COMPONENT:
	{
		component = new MeshComponent(this);
		break;
	}

	case Component::ComponentType::MATERIAL_COMPONENT:
	{
		component = new MaterialComponent(this);
		break;
	}

	default:
		break;
	}

	if (component != nullptr)
		components.push_back(component);

	return component;
}

void GameObject::setParent(GameObject* parent)
{

}




