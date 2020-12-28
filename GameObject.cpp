#include "GameObject.h"

GameObject::GameObject() : active{ true }, name{ "Game Object" }, components{ transform_, mesh_, material_ } {}

GameObject::~GameObject(){ children.clear(); }

void GameObject::Update()
{
	    
}

Component* GameObject::CreateComponent(ComponentType type) //Only in case that we press the "+" button in Inspector (1 argument)
{
	switch (type)
	{
	case TRANSFORM_COMPONENT:
	{
		TransformComponent* transformComp{ new TransformComponent {} };
		components.push_back(transformComp);
		return transformComp;
		break;
	}
	case MESH_COMPONENT:
	{
		MeshComponent* meshComp{ new MeshComponent {} };
		components.push_back(meshComp);
		return meshComp;
		break;
	}

	case MATERIAL_COMPONENT:
	{
		MaterialComponent* materialComp{ new MaterialComponent {} };
		components.push_back(materialComp);
		return materialComp;
		break;
	}
	default:
		break;
	}
}


