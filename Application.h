#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include <vector>

class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleCamera;
class ModuleEditor;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleTexture;
class ModuleDebugDraw;
class ModuleModel;
class ModuleScene;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleRenderExercise* exercise = nullptr;
	ModuleProgram* program = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleDebugDraw* draw = nullptr;
	ModuleModel* model = nullptr;
	ModuleScene* scene = nullptr;

private:

	std::vector<Module*> modules;

};

extern Application* App;
