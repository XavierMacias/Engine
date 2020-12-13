#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleModel.h"
#include "ModuleRenderExercise.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib/Math/Quat.h"

ModuleCamera::ModuleCamera():
	MAX_FPS{ 75.0f }
{
	position = float3(0, 2, -8);	
}

float4x4 ModuleCamera::setProjectionMatrix()
{
	float4x4 projectionGL = frustum.ProjectionMatrix();

	return projectionGL;
}

float4x4 ModuleCamera::setViewMatrix()
{
	float4x4 viewMatrix = frustum.ViewMatrix();

	return viewMatrix;
}

void ModuleCamera::SetInitUpFrontRight()
{
	Front.Normalized();
	Right = Cross(Front, WorldUp).Normalized();
	Up = Cross(Right, Front).Normalized();
}

void ModuleCamera::SetFrustum()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(nearPlane, farPlane);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD(90.0f), aspectRatio);

	frustum.SetPos(position);
	frustum.SetFront(Front);
	frustum.SetUp(Up);
}

// Called before render is available
bool ModuleCamera::Init()
{
	SetInitUpFrontRight();
	SetFrustum();
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*(setProjectionMatrix().v));

	return true;
}

// Called every draw update
update_status ModuleCamera::Update()
{	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*(setViewMatrix().v));

	MovementSpeed();
	WASD();	
	Orbit();
	Focus(false);

	//FPS
	CalculateFPS();
	float starTicks = SDL_GetTicks();
	float frameTicks = SDL_GetTicks() - starTicks;
	//Limit FPS
	if (1000.0f / MAX_FPS > frameTicks)
	{
		SDL_Delay(1000.0f / MAX_FPS - frameTicks);
	}

	SetFrustum();
	return UPDATE_CONTINUE;
}

void ModuleCamera::MouseMotionInput(float xoffset, float yoffset)
{
	xoffset *= movementSpeed;
	yoffset *= movementSpeed;

	oldpitch = pitch;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	CameraRotation(Right, pitch - oldpitch);
	CameraRotation(WorldUp, -xoffset);

	SetFrustum();
}

void ModuleCamera::CameraRotation(float3& axis, float angle)
{
	Quat rotationMatrix = Quat(axis, DEGTORAD(angle));

	Front = rotationMatrix * Front;
	Up = rotationMatrix * Up;
	Right = rotationMatrix * Right;
}

void ModuleCamera::WASD()
{
	if (App->input->Rpressed)
	{
		if (App->input->CheckKey(SDL_SCANCODE_W))
		{
			position += Front * movementSpeed;
		}
		if (App->input->CheckKey(SDL_SCANCODE_S))
		{
			position -= Front * movementSpeed;
		}

		if (App->input->CheckKey(SDL_SCANCODE_A))
		{
			position -= Right * movementSpeed;
		}

		if (App->input->CheckKey(SDL_SCANCODE_D))
		{
			position += Right * movementSpeed;
		}
	}
}

void ModuleCamera::MovementSpeed()
{
	movementSpeed = 4 * (1 / FPS);

	if (App->input->CheckKey(SDL_SCANCODE_LSHIFT))
	{
		movementSpeed *= 2;
	}
}

void ModuleCamera::WheelTransformation(int wheel)
{
	movementSpeed *= 2;
	position += wheel * Front * movementSpeed;
}

void ModuleCamera::Focus(bool newModel) 
{
	if ((App->input->CheckKey(SDL_SCANCODE_F) && !App->editor->GetFocused()) || newModel) {
		position.x = 0;
		position.y = App->model->ComputeCenter() / 2;
		position.z = 0 - 2 * App->model->ComputeCenter();
		frustum.SetFront(float3::unitZ);
		frustum.SetUp(float3::unitY);
		frustum.SetPos(position);
	}
}

void ModuleCamera::Orbit() 
{		
	if (App->input->Lpressed && App->input->CheckKey(SDL_SCANCODE_LALT) && !App->editor->GetFocused()) {
		vec up = frustum.Up().Normalized();
		vec right = frustum.WorldRight().Normalized();

		float3 camFocusVector = frustum.Pos();
		float3x3 rotationUp = frustum.ViewMatrix().RotatePart().RotateAxisAngle(up, movementSpeed);
		float3x3 rotationRight = frustum.ViewMatrix().RotatePart().RotateAxisAngle(right, movementSpeed);
		
		camFocusVector = camFocusVector * rotationUp;
		camFocusVector = camFocusVector * rotationRight;

		frustum.SetPos(camFocusVector);
		float3x3::LookAt(frustum.Front(), -frustum.Pos(), frustum.Up(), float3::unitY);
	}
}

void ModuleCamera::SetFOV(float aspectRadio) {
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspectRadio);
}

// Called before quitting
bool ModuleCamera::CleanUp()
{
	LOG("Destroying camera");

	return true;
}


float ModuleCamera::CalculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();

	float currentTicks;
	currentTicks = SDL_GetTicks();

	frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = frameTime;

	prevTicks = currentTicks;

	int count;

	currentFrame++;
	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}

	float frameTimeAverage = 0;

	for (int i = 0; i < count; i++)
	{
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0)
	{
		FPS = 1000.0f / frameTimeAverage;
	}
	else
	{
		FPS = 60.0f;
	}

	return FPS;
}