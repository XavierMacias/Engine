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
	~ModuleCamera();

	bool Init();
	update_status Update();
	bool CleanUp();
	float4x4 getProjection() { return projectionGL; }
	float4x4 getView() { return viewMatrix; }
	
private:

	void Yaw();
	void Pitch();
	void MoveForward();
	void MoveLateral();
	void MoveUp();
	void RotateMouse();
	void WheelMouse();
	void Rotate(float3x3 &rotationMatrix);

	Frustum frustum;
	float3 position;
	float speed, deltaTime, oldTime;
	float4x4 projectionGL, viewMatrix;
};

