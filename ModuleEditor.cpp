#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleModel.h"
#include "Globals.h"
#include "Application.h"
#include "SDL/include/SDL.h"

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
    focusAbout = focusConfig = focusConsole = focusProp = false;

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
    ImGuiIO& io = ImGui::GetIO();
    fps_log.push_back(io.Framerate);
    frames++;
    if (frames == 100) {
        fps_log.clear();
    }

    static bool config = false;
    static bool console = false;
    static bool about = false;
    static bool property = false;
    // MENU BAR
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Help")) {

        ImGui::MenuItem("Configuration Window", NULL, &config);
        ImGui::MenuItem("Console", NULL, &console);
        ImGui::MenuItem("Properties", NULL, &property);
        ImGui::MenuItem("About", NULL, &about);
        if (ImGui::MenuItem("Git")) {
            ShellExecute(0, 0, "https://github.com/XavierMacias/Engine/tree/Assignment1", 0, 0, SW_SHOWNORMAL);
        }
        if (ImGui::MenuItem("Quit")) {
            return UPDATE_STOP;
        }
        
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (config) {
        ConfigurationWindow();
    }
    if (console) {
        Console();
    }
    if (property) {
        Properties();
    }
    if (about) {
        About();
    }

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

void ModuleEditor::About() {

    ImGui::Begin("About");

    ImGui::Text("This is Engenius");
    ImGui::Text("Is an engine created by Xavier Macias");
    ImGui::Text("Please be good in the correction :)");

    ImGui::End();
    focusAbout = ImGui::IsWindowFocused();
}

void ModuleEditor::Properties() {
    ImGui::Begin("Properties");

    // Transformation
    if (ImGui::CollapsingHeader("Transformation")) {
        //Coords
        ImGui::Text("X: %f", 2.0);
        ImGui::Text("Y: %f", 2.0);
        ImGui::Text("Z: %f", 2.0);
    }

    // Geometry
    if (ImGui::CollapsingHeader("Geometry")) {

        ImGui::Text("Num Vertices: %d", App->model->GetVertices());
        ImGui::Text("Num Triangles: %d", App->model->GetTriangles());
        ImGui::Text("Num Meshes: %d", App->model->GetMeshes());
    }

    // Texture
    if (ImGui::CollapsingHeader("Texture")) {

        ImGui::Text("Texture Size: %f", 2.0);
    }
    focusProp = ImGui::IsWindowFocused();

    ImGui::End();
}

void ModuleEditor::ConfigurationWindow() {
    
    ImGui::Begin("Configuration");

    //FPS Graph
    ImGui::Text("FPS Graph");
    ImGui::PlotHistogram("##framerate",&fps_log[0],fps_log.size(),0,"FPS",0.0f,100.0f,ImVec2(310,100));

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

    // Info
    if (ImGui::CollapsingHeader("Information")) {

        ImGui::Text("CPUs: %d", SDL_GetCPUCount());
        ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
    }

    ImGui::SetWindowSize({ 250,300 });

    focusConfig = ImGui::IsWindowFocused();

    ImGui::End();
}

void ModuleEditor::Console() {
    ImGui::Begin("Console");
    ImGui::TextUnformatted(buf.begin());
    if (scrollToBottom) {
        ImGui::SetScrollHere(1.0f);
    }
    scrollToBottom = false;

    focusConsole = ImGui::IsWindowFocused();

    ImGui::End();
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	LOG("Destroying editor");

    fps_log.clear();
    ImGui_ImplOpenGL3_Shutdown(); 
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

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
    return (focusAbout || focusConfig || focusConsole || focusProp);
}