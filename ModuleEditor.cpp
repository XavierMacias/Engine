#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleModel.h"
#include "Globals.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"

ModuleEditor::ModuleEditor()
{

}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

// Called before render is available
bool ModuleEditor::Init()
{    

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->exercise->GetContext());
    ImGui_ImplOpenGL3_Init();
    focused = false;

	return true;
}

update_status ModuleEditor::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
    ImGui::Begin("Configuration");
    ImGui::Text("Times per second");
    ImGui::Text("Render: ");
    ImGui::Text("Window: ");
    ImGui::SetWindowSize({ 250,100 });
    if (ImGui::IsWindowFocused()) {
        focused = true;
    }
    else {
        focused = false;
    }

    ImGui::End();

    ImGui::Render();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	LOG("Destroying editor");

    ImGui_ImplOpenGL3_Shutdown(); 
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return true;
}