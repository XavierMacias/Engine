#ifndef __camera_component__
#define __camera_component__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3x3.h"
#include "MathGeoLib/Math/float4x4.h"

class CameraComponent : public Component
{
private:
	Frustum frustum;
	bool isCulling;
	bool isPespectiveProj; 
public:
	void updateFrustum();

	void setNearPLane(float distance);
	void setFarPlane(float distance);
	void setFOV(float fov);
	void setAspectRatio(float ar);

	const float3 getUp() { return frustum.Up(); }
	const float3 getFront() { return frustum.Front(); }
	float getNearPLane() const;
	float getFarPlane() const;
	float getFOV() const;
	float getAspectRatio() const;
};

#endif __camera_component__