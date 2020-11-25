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
    SDL_GetWindowSize(App->window->window, &w, &h);

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

    ImGui::SetWindowSize({ (float)(w / 2.3), (float)(h / 3.7) });
    
    focusAbout = ImGui::IsWindowFocused();

    ImGui::End();
}

void ModuleEditor::Properties() {

    ImGui::Begin("Properties");

    // Transformation
    if (ImGui::CollapsingHeader("Transformation")) {
        //Coords
        ImGui::Text("Front: %f %f %f", App->camera->getFront().x, App->camera->getFront().y, App->camera->getFront().z);
        ImGui::Text("Up: %f %f %f", App->camera->getUp().x, App->camera->getUp().y, App->camera->getUp().z);
        ImGui::Text("Position: %f %f %f", App->camera->getPosition().x, App->camera->getPosition().y, App->camera->getPosition().z);
    }

    // Geometry
    if (ImGui::CollapsingHeader("Geometry")) {

        // For every mesh, show vertices and faces
        ImGui::Text("Num Meshes: %d", App->model->GetMeshes());
        ImGui::Text("Num Vertices:");
        for (unsigned i = 0; i < App->model->GetMeshes(); ++i) {
            ImGui::Text("Mesh %d: %d", i+1, App->model->GetVertices(i));
        }
        ImGui::Text("Num Faces:");
        for (unsigned i = 0; i < App->model->GetMeshes(); ++i) {
            ImGui::Text("Mesh %d: %d", i+1, App->model->GetFaces(i));
            //ImGui::Image((ImTextureID)App->model->GetFaces(i), {2,2});
        }
        
    }

    // Texture
    if (ImGui::CollapsingHeader("Texture")) {
        for (int i = 0; i < App->model->GetNumMaterials(); ++i) {
            int textw = App->model->GetTextureWidth(i);
            int texth = App->model->GetTextureHeight(i);

            ImGui::Text("Texture %d size: %d %d", i+1, textw, texth);
            ImGui::Image((ImTextureID)App->model->GetMaterial(i), { (float)textw,(float)texth });
        }

    }

    ImGui::SetWindowSize({ (float)(w / 2),(float)(h / 1.6) });
    focusProp = ImGui::IsWindowFocused();

    ImGui::End();
}

void ModuleEditor::ConfigurationWindow() {

    SDL_version compiled;

    SDL_VERSION(&compiled);
    
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
        ImGui::Text("SDL version compiled: %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
        ImGui::Text("OpenGL version: %s", (char*)glGetString(GL_VERSION));
    }
    
    ImGui::SetWindowSize({ (float)(w / 2.15), (float)(h / 1.2) });

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

    ImGui::SetWindowSize({ (float)w, (float)(h / 2.4) });

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