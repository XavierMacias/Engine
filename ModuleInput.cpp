#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "SDL/include/SDL.h"

#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_sdl.h"


#define MAX_KEYS 300

ModuleInput::ModuleInput()
{
}

// Destructor
ModuleInput::~ModuleInput()
{
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	return ret;
}

// Called before the first frame
bool ModuleInput::Start()
{
	return true;
}

// Called each loop iteration
update_status ModuleInput::PreUpdate()
{
	SDL_Event event;
	
	float2 mouse_motion = { 0, 0 };

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return UPDATE_STOP;

		case SDL_WINDOWEVENT_MAXIMIZED:
			SDL_MaximizeWindow(App->window->window);

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				App->exercise->WindowResized(event.window.data1, event.window.data2);
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) { Lpressed = false; }
			if (event.button.button == SDL_BUTTON_RIGHT) { Rpressed = false; }
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) { Lpressed = true; }
			if (event.button.button == SDL_BUTTON_RIGHT) { Rpressed = true; }
			break;

		case SDL_MOUSEMOTION:
			mouse_motion.x = event.motion.xrel;
			mouse_motion.y = event.motion.yrel;
			
			break;

		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0)
			{
				App->camera->WheelTransformation(1);
			}
			else if (event.wheel.y < 0)
			{
				App->camera->WheelTransformation(-1);
			}
			break;

		case SDL_DROPFILE:
			char *dropped_filedir = event.drop.file;
			App->exercise->LoadModel(dropped_filedir);
			break;
		}
		//EDITOR
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			break;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
			break;
	}	
	if (Rpressed)
	{
		App->camera->MouseMotionInput(mouse_motion.x, -mouse_motion.y);
	}
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool ModuleInput::CheckKey(SDL_Scancode key)
{
	SDL_PumpEvents();

	keyboard = SDL_GetKeyboardState(NULL);

	return keyboard[key];
}