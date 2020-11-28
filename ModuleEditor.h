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
	std::vector<float> fps_log;
	int w, h;
};

#define NUM_EDITORS 2

