#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3x3.h"
#include "MathGeoLib/Math/float4x4.h"

#define DEGTORAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)

class ModuleCamera : public Module
{
public:
	ModuleCamera();

	float4x4 setProjectionMatrix();
	float4x4 setViewMatrix();
	void SetInitUpFrontRight();
	void SetFrustum();

	bool Init();	
	update_status Update();
	bool CleanUp();

	void SetFOV(float aspectRadio);
			
	const float3 getUp() { return frustum.Up(); }
	const float3 getFront() { return frustum.Front(); }	
	const float3 getRight() { return Right; }
	

	void Focus(bool newModel);
	void MovementSpeed();
	void WASD();
	void CameraRotation(float3& axis, float angle);
	void MouseMotionInput(float xoffset, float offset);
	void WheelTransformation(int wheel);
	void Orbit();
	float CalculateFPS();
	
private:	

	Frustum frustum;
	float3 position;	
	float3 Front{ float3::unitZ };
	float3 WorldUp{ float3::unitY };
	float3 Up;
	float3 Right;
	float movementSpeed;
	float4x4 projectionGL, viewMatrix;

	float yaw{ -90.0f };
	float pitch{ 0.0f };
	float oldpitch{ 0.0f };

	float nearPlane{ 0.1f };
	float farPlane{ 200.0f };
	float aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	
	
public:
	float FPS;
	float frameTime;
	float MAX_FPS;
	float deltaTime = 0.0f;

};

