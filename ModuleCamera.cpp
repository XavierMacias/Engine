#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "SDL.h"
#include "GL/glew.h"
#include <chrono>

ModuleCamera::ModuleCamera()
{
	position = float3(0, 1, -2);
	speed = 0.002;
	oldTime = 0.0;
}

// Destructor
ModuleCamera::~ModuleCamera()
{
}

// Called before render is available
bool ModuleCamera::Init()
{
	LOG("Creating Camera context");

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);
	frustum.SetPos(position);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	return true;
}

void ModuleCamera::Yaw() {

	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT)) {
		float3x3 rotationMatrix = frustum.WorldMatrix().RotatePart().RotateY(sped);
		Rotate(rotationMatrix);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT)) {
		float3x3 rotationMatrix = frustum.WorldMatrix().RotatePart().RotateY(-sped);
		Rotate(rotationMatrix);
	}

}

void ModuleCamera::Pitch() {

	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP)) {
		vec oldFront = (frustum.Front() * cos(sped) + frustum.Up() * sin(sped)).Normalized();
		vec oldUp = frustum.WorldRight().Cross(oldFront);
		frustum.SetFront(oldFront);
		frustum.SetUp(oldUp);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN)) {
		vec oldFront = (frustum.Front() * cos(-sped) + frustum.Up() * sin(-sped)).Normalized();
		vec oldUp = frustum.WorldRight().Cross(oldFront);
		frustum.SetFront(oldFront);
		frustum.SetUp(oldUp);
	}

}

void ModuleCamera::MoveForward() {

	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}

	if (App->input->GetKey(SDL_SCANCODE_W)) {
		frustum.Translate(frustum.Front() * sped);
		position = frustum.Pos();
		frustum.SetPos(position);
	}

	if (App->input->GetKey(SDL_SCANCODE_S)) {
		frustum.Translate(frustum.Front() * -sped);
		position = frustum.Pos();
		frustum.SetPos(position);
	}

}

void ModuleCamera::MoveLateral() {

	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}

	if (App->input->GetKey(SDL_SCANCODE_A)) {
		frustum.Translate(frustum.WorldRight() * -sped);
		position = frustum.Pos();
		frustum.SetPos(position);
	}

	if (App->input->GetKey(SDL_SCANCODE_D)) {
		frustum.Translate(frustum.WorldRight() * sped);
		position = frustum.Pos();
		frustum.SetPos(position);
	}

}

void ModuleCamera::MoveUp() {
	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}

	if (App->input->GetKey(SDL_SCANCODE_Q)) {
		position.y += sped;
		frustum.SetPos(position);
	}

	if (App->input->GetKey(SDL_SCANCODE_E)) {
		position.y -= sped;
		frustum.SetPos(position);
	}
}

void ModuleCamera::RotateMouse() {
	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) && !App->editor->GetFocused()) {
		iPoint mouse = App->input->GetMouseMotion();

		Rotate(frustum.WorldMatrix().RotatePart().RotateY(mouse.x * sped * deltaTime));

		vec oldFront = (frustum.Front() * cos(mouse.y * sped * deltaTime) + frustum.Up() * sin(mouse.y * sped * deltaTime)).Normalized();
		vec oldUp = frustum.WorldRight().Cross(oldFront);
		frustum.SetFront(oldFront);
		frustum.SetUp(oldUp);
	}
}

void ModuleCamera::WheelMouse() {
	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}
	if (App->input->GetWheel() > 0) {
		frustum.Translate(frustum.Front() * sped * 30);
		position = frustum.Pos();
		frustum.SetPos(position);
	}
	else if (App->input->GetWheel() < 0) {
		frustum.Translate(frustum.Front() * -sped * 30);
		position = frustum.Pos();
		frustum.SetPos(position);
	}
}

// Called every draw update
update_status ModuleCamera::Update()
{
	deltaTime = clock() - oldTime;
	double fps = (1.0 / deltaTime) * 1000;
	oldTime = clock();

	projectionGL = frustum.ProjectionMatrix();
	viewMatrix = frustum.ViewMatrix();

	MoveForward();
	MoveLateral();
	MoveUp();
	Pitch();
	Yaw();
	RotateMouse();
	WheelMouse();

	App->input->SetWheel(0);

	return UPDATE_CONTINUE;
}

void ModuleCamera::Rotate(float3x3& rotation) {
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotation.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotation.MulDir(oldUp));
}

// Called before quitting
bool ModuleCamera::CleanUp()
{
	LOG("Destroying camera");

	return true;
}

