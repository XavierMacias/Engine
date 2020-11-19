#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"
#include "ModuleDebugDraw.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/float3x3.h"

ModuleRenderExercise::ModuleRenderExercise()
{

}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
}

// Called before render is available
bool ModuleRenderExercise::Init()
{

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	context = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();


	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise
	
	return true;
}

bool ModuleRenderExercise::Start() {
	App->model->Load("BakerHouse.fbx");
	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	int w, h;

	SDL_GetWindowSize(App->window->window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{

	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	App->draw->Draw(App->camera->getView(), App->camera->getProjection(), w, h);

	App->model->Draw();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderExercise::WindowResized(unsigned width, unsigned height)
{
}