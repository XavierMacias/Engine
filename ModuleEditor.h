#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"


class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void ConfigurationWindow(int index);
	void Console(int index);
	void AddLog(const char* fmt);
	bool GetFocused();
private:
	ImGuiTextBuffer buf;
	bool scrollToBottom;
	std::vector<bool> focuseds;
};

#define NUM_EDITORS 2

