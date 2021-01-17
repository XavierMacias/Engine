#ifndef __Game_Object__
#define __Game_Object__

#include <string>
#include <vector>
#include<list>

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/Quat.h"
#include "MathGeoLib/Math/float4x4.h"

class GameObject
{
	friend class Component;

private:
	GameObject* parent;
	UID uid;
	Quat rotation = Quat::identity;
	float4x4 localTransform;

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
	Component* CreateComponent(Component::ComponentType type, std::string name);
	void GetComponent(Component::ComponentType type, std::vector<Component*>& results);
	GameObject* GetParent() { return parent; }
	void setParent(GameObject* parent);
	bool GetRoot() { return isRoot; }
	int GetUid() { return uid; }


	bool isActive();
	void SetActive(bool act);

	void SetName(char* n) { name = n; }
	void SetLocalTransform(const float4x4& transform);
	void SetLocalPosition(const float pos[3]);
	void SetLocalRotation(const float rot[3]);
	void SetScale(const float scl[3]);
	
	float3 GetLocalPosition();
	Quat GetLocalRotation();
	float3 GetScale();

	bool HasMeshComponent();
	bool HasMaterialComponent();

};

#endif __Game_Object__
