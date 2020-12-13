﻿#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"
#include "ModuleDebugDraw.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/float3x3.h"
#include "assimp/cimport.h"

ModuleRenderExercise::ModuleRenderExercise()	
{

}

void myCallback(const char* msg, char* userData) {
	App->editor->AddLog(msg);
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

	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
	
	return true;
	
}

bool ModuleRenderExercise::Start() {
	LoadModel("BakerHouse.fbx");
	return true;
}

void ModuleRenderExercise::LoadModel(char* filename) {
	if (getFileExt((std::string)filename) == "fbx") {
		App->model->CleanUp();
		App->model->Load(filename);
		//App->camera->Focus(true);
		//SDL_free(filename);
	}
	else if (getFileExt((std::string)filename) == "png" || getFileExt((std::string)filename) == "dds") {
		for (int i = 0; i < App->model->GetNumMaterials(); ++i) {
			App->texture->FreeTexture(App->model->GetMaterial(i));
		}
		App->model->LoadTexture(filename);
	}
	else {
		App->editor->AddLog("This isn't a FBX file");
	}
}

std::string ModuleRenderExercise::getFileExt(const std::string s) {

	size_t i = s.rfind('.', s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}

update_status ModuleRenderExercise::PreUpdate()
{
	int w, h;

	SDL_GetWindowSize(App->window->window, &w, &h);
	glViewport(0, 0, w, h);

	background.x = App->editor->bGround[0];
	background.y = App->editor->bGround[1];
	background.z = App->editor->bGround[2];
	background.w = App->editor->bGround[3];

	glClearColor(background[0], background[1], background[2], background[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{	
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	grid.x = App->editor->gridColor[0];
	grid.y = App->editor->gridColor[1];
	grid.z = App->editor->gridColor[2];

	App->draw->Draw(App->camera->setViewMatrix(), App->camera->setProjectionMatrix(), w, h, grid);

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
	App->camera->SetFOV(float(width) / float(height));
}

void ModuleRenderExercise::FrameBuffer()
{
	//Frame buffer object
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//Texture color buffer
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	//Render buffer object
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);				
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);	
																									
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}
