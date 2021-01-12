#include "GameObject.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "Application.h"

GameObject::GameObject(GameObject* parent, const char* name, bool isRoot) : parent(parent), name(name), isRoot(isRoot)
{
	uid = App->random->Int();	
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

void GameObject::setParent(GameObject* p)
{
	parent = p;
}

void GameObject::SetLocalTransform(const float4x4& transform)
{
	transform.Decompose(position, rotation, scale);
	rotationEditor = rotation.ToEulerXYZ().Abs();
}

void GameObject::SetLocalPosition(const float3& pos)
{
	position = pos;
}

void GameObject::SetLocalRotation(const Quat& rotation)
{
	this->rotation = rotation;
	rotationEditor = rotation.ToEulerXYZ().Abs();
}

void GameObject::SetLocalScale(const float3& scale)
{
	this->scale = scale;
}

bool GameObject::HasMeshComponent()
{
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i]->MESH_COMPONENT)
		{
			return true;
		}
	}
}

bool GameObject::HasMaterialComponent()
{
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i]->MATERIAL_COMPONENT)
		{
			return true;
		}
	}
}

bool GameObject::isActive()
{
	return active;
}

void GameObject::SetActive(bool act)
{
	if (this->active != active)
	{
		this->active = active;
	}
}
