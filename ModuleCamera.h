#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3x3.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleCamera : public Module
{
public:
	ModuleCamera();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();
	void SetFOV(float aspectRadio);
	const float4x4 getProjection() { return projectionGL; }
	const float4x4 getView() { return viewMatrix; }
	const float3 getPosition() { return position; }
	const float3 getUp() { return frustum.Up(); }
	const float3 getFront() { return frustum.Front(); }
	const float getDeltaTime() { return deltaTime; }
	void Focus(bool newModel);
	
private:

	void Yaw();
	void Pitch();
	void MoveForward();
	void MoveLateral();
	void MoveUp();
	void RotateMouse();
	void WheelMouse();
	void Orbit();
	void Rotate(float3x3 &rotationMatrix);

	Frustum frustum;
	float3 position;
	float speed, deltaTime, oldTime;
	float4x4 projectionGL, viewMatrix;
};

