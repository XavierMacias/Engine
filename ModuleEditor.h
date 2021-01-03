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
	void Project();	
	void Hierarchy();
	
	
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

	//Open Collapsing header
	bool open_transformation{ false };
	bool open_geometry{ false };
	bool open_texture{ false };
	bool open_camera{ false };
	
	//Meshes
	char* meshName{"BakerHouse"};

	//Temporal 
	float position[3]{ 0.0f, 0.0f, 0.0f };
	float rotation[3]{ 0.0f, 0.0f, 0.0f };
	float scale[3]{ 0.0f, 0.0f, 0.0f };

public:
	float* gridColor{ new float[3]{1.000000f, 0.647059f, 0.000000f} };
	float* bGround{ new float[4]{0.1f, 0.1f, 0.1f, 0.1f } };
	float* ambient_color{ new float[3]{0.4f, 0.4f, 0.4f } };
	float* light_color{ new float[3]{1.0f, 1.0f, 1.0f } };
	float* light_direction{ new float[3]{1.0f, 0.0f, 0.0f } };
	float nSpec = 2.0f;
	float kSpec = 0.04f;
	float kDiff = 0.7f;
};

#define NUM_EDITORS 2

