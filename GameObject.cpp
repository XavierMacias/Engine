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

Component* GameObject::CreateComponent(Component::ComponentType type, std::string name)
{
	Component* component = nullptr;

	switch (type)
	{	
	case Component::ComponentType::MESH_COMPONENT:
	{
		component = new MeshComponent(this, name);
		break;
	}

	case Component::ComponentType::MATERIAL_COMPONENT:
	{
		component = new MaterialComponent(this, name);
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

void GameObject::SetLocalPosition(const float pos[3])
{
	position.x = pos[0];
	position.y = pos[1];
	position.z = pos[2];
}

void GameObject::SetLocalRotation(const float rot[3])
{
	rotation.x = rot[0];
	rotation.y = rot[1];
	rotation.z = rot[2];
	rotation.w = 1.0f;
}

void GameObject::SetScale(const float scl[3])
{
	scale.x = scl[0];
	scale.y = scl[1];
	scale.z = scl[2];
}

float3 GameObject::GetLocalPosition()
{
	return position;	
}

Quat GameObject::GetLocalRotation()
{
	return rotation;
}

float3 GameObject::GetScale()
{
	return scale;
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

void GameObject::GetComponent(Component::ComponentType type, std::vector<Component*>& results)
{
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); ++it)
		if ((*it)->GetType() == type)
			results.push_back(*it);
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
