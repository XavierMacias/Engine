#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH;
		int height = SCREEN_HEIGHT;
		flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetFlags(SDL_WindowFlags flag, bool state) {
	switch (flag) {
		case SDL_WINDOW_FULLSCREEN:
			SDL_SetWindowFullscreen(window, state);
			break;
		case SDL_WINDOW_RESIZABLE:
			SDL_SetWindowResizable(window, (SDL_bool)state);
			break;
		case SDL_WINDOW_BORDERLESS:
			if (state) SDL_SetWindowBordered(window, SDL_FALSE);
			else SDL_SetWindowBordered(window, SDL_TRUE);
			break;
		default:
			break;
	}
}

void ModuleWindow::SetBrightness(float bright) {
	SDL_SetWindowBrightness(window, bright);
}

void ModuleWindow::SetWindowSize(int width, int height) {
	SDL_SetWindowSize(window, width, height);
	App->camera->SetFOV(float(width)/float(height));

}
