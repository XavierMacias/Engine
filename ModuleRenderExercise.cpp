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

	/*float vtx_data[] = {
		-1.0f, -1.0f, 0.0f, // ← v0 pos
		1.0f, 1.0f, 0.0f, // ← v1 pos
		-1.0f, 1.0f, 0.0f, // ← v2 pos
		
		-1.0f, 1.0f, 0.0f, // ← v2 pos
		1.0f, -1.0f, 0.0f,// ← v1 pos
		1.0f, 1.0f, 0.0f,// ← v3 pos

		//0.0f, 0.0f, // ← v0 texcoord
		//1.0f, 0.0f, // ← v1 texcoord
		//0.0f, 1.0f, // ← v2 texcoord

		//0.0f, 1.0f, // ← v2 texcoord
		//1.0f, 0.0f, // ← v1 texcoord
		//1.0f, 1.0f  // ← v3 texcoord
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);*/

	App->model->Load("BakerHouse.fbx");
	return true;
}

// model load baker house en start

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
	/*float4x4 model, view, projection;

	projection = App->camera->getProjection();
	view = App->camera->getView();
	model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
		float4x4::RotateZ(pi / 4.0f),
		float3(2.0f, 1.0f, 0.0f));*/

	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	App->draw->Draw(App->camera->getView(), App->camera->getProjection(), w, h);

	/*glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,(void*)(sizeof(float) * 6 * 3));


	glUseProgram(App->program->GetProgram());
	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, App->texture->getTexture());

	//glUniform1i(glGetUniformLocation(App->program->GetProgram(), "mytexture"), 0);
	glUniformMatrix4fv(glGetUniformLocation(App->program->GetProgram(), "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->GetProgram(), "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->GetProgram(), "proj"), 1, GL_TRUE, &projection[0][0]);
	// TODO: bind buffer and vertex attributes
	glDrawArrays(GL_TRIANGLES, 0, 6);*/

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
	glDeleteBuffers(1, &vbo);
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderExercise::WindowResized(unsigned width, unsigned height)
{
}