#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleModel.h"
#include "Globals.h"
#include "Application.h"

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
    AddLog("Welcome to console");
    focuseds.resize(NUM_EDITORS);

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
    ConfigurationWindow(0);
    Console(1);

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

void ModuleEditor::ConfigurationWindow(int index) {

    ImGui::Begin("Configuration");
    ImGui::Text("Times per second");
    static bool fullscreen = false;
    static bool border = false;
    static bool resizable = true;
    
    // Window
    if (ImGui::CollapsingHeader("Window")) {
        //Brightness
        static float brightness = 1.0f;
        ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f);
        App->window->SetBrightness(brightness);

        //Width
        static int width = SCREEN_WIDTH;
        ImGui::SliderInt("Width", &width, 100, 1000);

        //Height
        static int height = SCREEN_HEIGHT;
        ImGui::SliderInt("Height", &height, 100, 800);

        App->window->SetWindowSize(width, height);

        // Fullscreen
        if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
            App->window->SetFlags(SDL_WINDOW_FULLSCREEN, fullscreen);
        }

        // Borderless
        if (ImGui::Checkbox("Borderless", &border)) {
            App->window->SetFlags(SDL_WINDOW_BORDERLESS, border);
        }

        // Resizable
        if (ImGui::Checkbox("Resizable", &resizable)) {
            App->window->SetFlags(SDL_WINDOW_RESIZABLE, resizable);
        }
        
    }


    ImGui::SetWindowSize({ 250,300 });

    focuseds[index] = ImGui::IsWindowFocused();

    ImGui::End();
}

void ModuleEditor::Console(int index) {
    ImGui::Begin("Console");
    ImGui::TextUnformatted(buf.begin());
    if (scrollToBottom) {
        ImGui::SetScrollHere(1.0f);
    }
    scrollToBottom = false;

    focuseds[index] = ImGui::IsWindowFocused();

    ImGui::End();
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	LOG("Destroying editor");

    ImGui_ImplOpenGL3_Shutdown(); 
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    focuseds.clear();

	return true;
}

void ModuleEditor::AddLog(const char* fmt)
{
    va_list args;
    va_start(args, fmt);
    buf.appendf(fmt, args);
    va_end(args);
    scrollToBottom = true;
}

bool ModuleEditor::GetFocused() {
    for (unsigned i = 0; i < focuseds.size(); ++i) {
        if (focuseds[i]) {
            return true;
        }
    }
    return false;
}