#pragma once
#include "Module.h"
#include "Globals.h"

#include <iostream>
#include <string>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	void LoadModel(char* filename);
	void* GetContext() { return context; }

private:
	std::string getFileExt(const std::string s);

	void* context;
};

