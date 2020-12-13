#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_internal.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();	
	bool CleanUp();
	void AddLog(const char* fmt);
	bool GetFocused();

private:
	void ConfigurationWindow();
	void Console();
	void About();
	void Properties();
		
	ImGuiTextBuffer buf;
	bool scrollToBottom;
	bool focusConfig, focusAbout, focusConsole, focusProp;	
	int w, h;

	bool show_console{ true };
	bool show_app_prop{ true };
	bool show_app_config{ true };
	bool show_app_about{ false };		

public:
	float* gridColor{ new float[3]{1.000000f, 0.647059f, 0.000000f} };
	float* bGround{ new float[4]{0.1f, 0.1f, 0.1f, 0.1f } };
};

#define NUM_EDITORS 2

