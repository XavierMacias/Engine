#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleModel.h"
#include "SDL.h"
#include "GL/glew.h"

ModuleCamera::ModuleCamera()
{
	position = float3(0, 1, -2);
	speed = 0.002;
	oldTime = 0.0f;
	deltaTime = 0.0f;
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

bool ModuleCamera::Start() {
	
	position.z = 0 - 4*App->model->GetScale();
	frustum.SetPos(position);
	return true;
}

void ModuleCamera::Yaw() {

	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) && !App->editor->GetFocused()) {
		float3x3 rotationMatrix = frustum.WorldMatrix().RotatePart().RotateY(sped);
		Rotate(rotationMatrix);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) && !App->editor->GetFocused()) {
		float3x3 rotationMatrix = frustum.WorldMatrix().RotatePart().RotateY(-sped);
		Rotate(rotationMatrix);
	}
}

void ModuleCamera::Pitch() {

	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) && !App->editor->GetFocused()) {
		vec oldFront = (frustum.Front() * cos(sped) + frustum.Up() * sin(sped)).Normalized();
		vec oldUp = frustum.WorldRight().Cross(oldFront);
		frustum.SetFront(oldFront);
		frustum.SetUp(oldUp);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) && !App->editor->GetFocused()) {
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

	if (App->input->GetKey(SDL_SCANCODE_W) && !App->editor->GetFocused()) {
		frustum.Translate(frustum.Front() * sped);
		position = frustum.Pos();
		frustum.SetPos(position);
	}

	if (App->input->GetKey(SDL_SCANCODE_S) && !App->editor->GetFocused()) {
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

	if (App->input->GetKey(SDL_SCANCODE_A) && !App->editor->GetFocused()) {
		frustum.Translate(frustum.WorldRight() * -sped);
		position = frustum.Pos();
		frustum.SetPos(position);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) && !App->editor->GetFocused()) {
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

	if (App->input->GetKey(SDL_SCANCODE_Q) && !App->editor->GetFocused()) {
		position.y += sped;
		frustum.SetPos(position);
	}

	if (App->input->GetKey(SDL_SCANCODE_E) && !App->editor->GetFocused()) {
		position.y -= sped;
		frustum.SetPos(position);
	}
}

void ModuleCamera::RotateMouse() {
	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) && !App->editor->GetFocused()) {
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
	if (App->input->GetWheel() > 0 && !App->editor->GetFocused()) {
		frustum.Translate(frustum.Front() * sped * 30);
		position = frustum.Pos();
		frustum.SetPos(position);
	}
	else if (App->input->GetWheel() < 0 && !App->editor->GetFocused()) {
		frustum.Translate(frustum.Front() * -sped * 30);
		position = frustum.Pos();
		frustum.SetPos(position);
	}
}

void ModuleCamera::Focus() {
	if (App->input->GetKey(SDL_SCANCODE_F) && !App->editor->GetFocused()) {
		position.x = 0;
		position.y = 0;
		position.z = 0 - 4 * App->model->GetScale();
		frustum.SetPos(position);
	}
}

void ModuleCamera::Orbit() {
	float sped = speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		sped *= 2;
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) && App->input->GetKey(SDL_SCANCODE_LALT) && !App->editor->GetFocused()) {
		vec up = frustum.Up().Normalized();
		vec right = frustum.WorldRight().Normalized();

		float3 camFocusVector = frustum.Pos();
		float3x3 rotationUp = frustum.ViewMatrix().RotatePart().RotateAxisAngle(up, sped);
		float3x3 rotationRight = frustum.ViewMatrix().RotatePart().RotateAxisAngle(right, sped);
		
		camFocusVector = camFocusVector * rotationUp;
		camFocusVector = camFocusVector * rotationRight;

		frustum.SetPos(camFocusVector);
		float3x3::LookAt(frustum.Front(), -frustum.Pos(), frustum.Up(), float3::unitY);

	}
}

// Called every draw update
update_status ModuleCamera::Update()
{
	deltaTime = clock() - oldTime;
	//double fps = (1.0 / deltaTime) * 1000;
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
	Orbit();
	Focus();

	App->input->SetWheel(0);

	return UPDATE_CONTINUE;
}

void ModuleCamera::Rotate(float3x3& rotation) {
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotation.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotation.MulDir(oldUp));
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

