#ifndef __transform_component__
#define __transform_component__

#include "Component.h"
#include "MathGeoLib/Math/float3.h"

class TransformComponent : public Component
{
private:
	float3 position;
	float3 rotation;
	float3 scale;

public:
	TransformComponent(GameObject* gameObject);
	~TransformComponent();	

	void setPosition(const float3& pos);
	void setRotation(const float3& rot);
	void setScale(const float3& sc);

	float3 getPosition() { return position; };
	float3 getRotation() { return rotation; };
	float3 getScale() { return scale; };
};

#endif __transform_component__
