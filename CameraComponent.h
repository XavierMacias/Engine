#ifndef __camera_component__
#define __camera_component__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3x3.h"
#include "MathGeoLib/Math/float4x4.h"

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* gameObject, std::string name);
	~CameraComponent();
};

#endif __camera_component__