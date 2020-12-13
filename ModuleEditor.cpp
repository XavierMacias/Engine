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
    focusAbout = focusConfig = focusConsole = focusProp = scrollToBottom = false;
}

// Called before render is available
bool ModuleEditor::Init()
{    

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->exercise->GetContext());
    ImGui_ImplOpenGL3_Init();
    AddLog("Welcome to console");
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
	return true;
}

update_status ModuleEditor::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
    ImGuiIO& io = ImGui::GetIO();

    //Disable IMGUI keys 
    io.KeyAlt = false;
    io.KeyShift = false;

    //Docking
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("DockSpac", NULL, window_flags);           

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    }
        
    ImGui::End();

    //Frambuffer
    //ImGui::Begin("Render");

    //ImTextureID my_tex_id = io.Fonts->TexID;
    //float my_tex_w = 800;
    //float my_tex_h =600;
    //    
    //ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    //ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    //ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    //ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // 50% opaque white
    //ImGui::Image((ImTextureID)App->exercise->textureColorbuffer, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

    //ImGui::End();
        

    // MENU BAR
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Help")) {

        ImGui::MenuItem("Configuration Window", NULL, &show_app_config);
        ImGui::MenuItem("Console", NULL, &show_console);
        ImGui::MenuItem("Properties", NULL, &show_app_prop);
        ImGui::MenuItem("About", NULL, &show_app_about);
        //ImGui::MenuItem("About", NULL, &about);
        if (ImGui::MenuItem("Git")) {
            ShellExecute(0, 0, "https://github.com/XavierMacias/Engine/tree/Assignment1", 0, 0, SW_SHOWNORMAL);
        }
        if (ImGui::MenuItem("Quit")) {
            return UPDATE_STOP;
        }
        
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (show_app_config) { ConfigurationWindow(); }
    if (show_console) { Console(); }
    if (show_app_prop) { Properties();}
    if (show_app_about) { About(); }

    ImGui::Render();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
    
	return UPDATE_CONTINUE;
}

void ModuleEditor::About() {

    ImGui::Begin("About");

    ImGui::Text("This is Engenius");
    ImGui::Text("Is an engine created by Xavier Macias and Rober Gil");
    
    ImGui::End();
}

void ModuleEditor::Properties() {

    ImGui::Begin("Properties", &show_app_prop);

    // Transformation
    if (ImGui::CollapsingHeader("Transformation")) {
        //Coords
        ImGui::Text("Front: %f %f %f", App->camera->getFront().x, App->camera->getFront().y, App->camera->getFront().z);
        ImGui::Text("Up: %f %f %f", App->camera->getUp().x, App->camera->getUp().y, App->camera->getUp().z);
        ImGui::Text("Right: %f %f %f", App->camera->getRight().x, App->camera->getRight().y, App->camera->getRight().z);
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
    
    ImGui::Begin("Configuration", &show_app_config);

    static char engine_name[12] = TITLE;
    static char organization[18] = "UPC BarcelonaTECH";
    static float FPS[100] = {};
    static float millisecond[100] = {};
    static int index = 0;
    static double refresh_time = 0.0;

    //FPS Graph
    ImGui::Separator();
    if (ImGui::CollapsingHeader("Application"))
    {
        static bool animate = true;
        ImGui::Checkbox("Animate", &animate);

        ImGui::InputText("Engine", engine_name, IM_ARRAYSIZE(engine_name));
        ImGui::InputText("Organization", organization, IM_ARRAYSIZE(organization));

        if (!animate || refresh_time == 0.0)
        {
            refresh_time = ImGui::GetTime();
        }

        while (refresh_time < ImGui::GetTime())
        {
            FPS[index] = App->camera->FPS;

            millisecond[index] = App->camera->frameTime;

            refresh_time += 1.0f / 60.0f;
            if (index < IM_ARRAYSIZE(FPS))
            {
                ++index;
            }
            else
            {
                index = 0;
            }
        }

        float FPSaverage = 0.0f;
        float MSaverage = 0.0f;

        for (int n = 0; n < IM_ARRAYSIZE(FPS); ++n)
        {
            FPSaverage += FPS[n];
            MSaverage += millisecond[n];
        }

        FPSaverage /= (float)IM_ARRAYSIZE(FPS);
        MSaverage /= (float)IM_ARRAYSIZE(millisecond);


        char title[25];
        sprintf(title, "Framerate %.1f", FPSaverage);
        ImGui::PlotHistogram("##framerate", FPS, IM_ARRAYSIZE(FPS), 0, title, 0.0f, 100.0f, ImVec2(310, 100.0f));
        sprintf(title, "Milliseconds %.1f", MSaverage);
        ImGui::PlotHistogram("##milliseconds", millisecond, IM_ARRAYSIZE(millisecond), 0, title, 0.0f, 40.0f, ImVec2(310, 100.0f));
    }

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

    if (ImGui::CollapsingHeader("Render"))
    {
        ImGui::SliderFloat3("Grid color", gridColor, 0.0f, 1.0f);
        ImGui::SliderFloat4("Background color", bGround, 0.0f, 1.0f);
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
    ImGui::Begin("Console", &show_console);
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

    buf.clear();    
    ImGui_ImplOpenGL3_Shutdown(); 
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    delete[] gridColor;
    delete[]bGround;

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

