#ifndef __Game_Object__
#define __Game_Object__

#include <string>
#include <vector>
#include<list>

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/Math/Quat.h"
#include "MathGeoLib/Math/float4x4.h"

class GameObject
{
	friend class Component;

private:
	GameObject* parent;
	UID uid;
	Quat rotation = Quat::identity;

public:
	float3 position = float3::zero;
	float3 rotationEditor = float3::zero;
	float3 scale = float3::zero;
	
public:
	bool active = true;
	const char* name;
	bool isRoot;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

public:
	GameObject(GameObject* parent, const char* name, bool isRoot);
	~GameObject();

	void Update();
	Component* CreateComponent(Component::ComponentType type);
	GameObject* GetParent() { return parent; }
	void setParent(GameObject* parent);
	bool GetRoot() { return isRoot; }
	int GetUid() { return uid; }

	bool isActive();
	void SetActive(bool act);

	void SetName(char* n) { name = n; }
	void SetLocalTransform(const float4x4& transform);
	void SetLocalPosition(const float3& pos);	
	void SetLocalRotation(const Quat& rotation);
	void SetLocalScale(const float3& scale);
	
	bool HasMeshComponent();
	bool HasMaterialComponent();

};

#endif __Game_Object__
