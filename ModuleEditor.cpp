#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleModel.h"
#include "Globals.h"
#include "Application.h"
#include "SDL/include/SDL.h"
#include "ModuleScene.h"

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
    objIndex = 0;
    open_transformation = true;
    open_geometry = true;
    open_texture = true;
    open_camera = true;

    root = App->scene->CreateGameObject(NULL, "Scene", true);
    /*GameObject* go1 = App->scene->CreateGameObject(NULL, "GameObject1");
    GameObject* go2 = App->scene->CreateGameObject(NULL, "GameObject2");
    GameObject* go3 = App->scene->CreateGameObject(go2, "GameChild3");
    GameObject* go4 = App->scene->CreateGameObject(go2, "GameChild4");
    GameObject* go5 = App->scene->CreateGameObject(go4, "GrandChild5");
    GameObject* go6 = App->scene->CreateGameObject(NULL, "GameObject6");
    GameObject* go7 = App->scene->CreateGameObject(go1, "GameChild7");
    GameObject* go8 = App->scene->CreateGameObject(go7, "GrandChild8");
    GameObject* go9 = App->scene->CreateGameObject(go5, "GrandGran9");*/

	return true;
}

update_status ModuleEditor::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    /*ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;*/
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
    io.KeyCtrl = false;

    int w, h;
   
    //Docking window
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    ImGui::Begin("DockSpac", NULL, window_flags);           
    SDL_GetWindowSize(App->window->window, &w, &h);
    ImGui::SetWindowSize({ static_cast<float>(w), static_cast<float>(h) });
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    }

    if (ImGui::Begin("Scene"))
    {        
        ImVec2 window_size = ImVec2{ ImGui::GetWindowWidth() + 10, ImGui::GetWindowHeight() - 35};
        ImGui::Image(ImTextureID(App->exercise->textureColorbuffer), window_size, ImVec2(0,1), ImVec2(1,0));

        ImGui::End();

    }

    if (ImGui::Begin("Game"))
    {        
        ImGui::End();
    }
        
    if(ImGui::BeginMenuBar())
        {
            if (ImGui::Button("Hand Tool"))
            {   
                App->camera->HandTool = true;
            }
            if (ImGui::Button("Move Tool"))
            {
                App->camera->HandTool = false;
            }
            if (ImGui::Button("Start"))
            {               
                //Camera object, avoid to do changes
            }
            
            if (ImGui::Button("Pause"))
            {

            }
            if (ImGui::Button("Tick"))
            {

            }
        }

    ImGui::EndMenuBar();
    ImGui::End();
    Hierarchy();
    Project();
          
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
    ImGui::Text("It's an engine created by Xavier Macias and Robert Gil");
    
    ImGui::End();
}

//GameObject Hierarchy
void ModuleEditor::Hierarchy()
{
    GameObject* go = nullptr;
    ImGui::Begin("Hierarchy");
    gameobjects = App->scene->GetGameObjects();
    
    if (ImGui::Button("Create empty GameObject"))
        //ImGui::OpenPopup("Create");
        App->scene->CreateGameObject(root, "New GameObject", false);
        

    if (ImGui::BeginPopupModal("Create", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Button("Create")) {
            App->scene->CreateGameObject(root, "New GameObject", false);
            ImGui::CloseCurrentPopup();
            //createName[64] = (char)"New GameObject";
        }
        //char createName[64] = "New GameObject";
        //ImGui::InputText("Name", createName, 64);
        if (ImGui::Button("No"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    //if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen))
    //{
    GetHierarchy(root);
        //ImGui::TreePop();
    //}
    
    ConfigurationWindow();
    ImGui::End();
}


void ModuleEditor::GetHierarchy(GameObject* current) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    if (current == selectedObject) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    if (current->children.size() == 0) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (ImGui::TreeNodeEx(current->name, flags)) {
        if (ImGui::IsItemClicked(ImGuiMouseButton(0))) {
            selectedObject = current;
        }
        if (ImGui::IsItemClicked(ImGuiMouseButton(1))) {
            selectedObject = current;
            ImGui::OpenPopup("GameObject");
        }

        if (ImGui::BeginDragDropSource()) {
            selectedObject = current;
            ImGui::SetDragDropPayload("Source", &current, sizeof(GameObject*));
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget()) {
            if (ImGui::AcceptDragDropPayload("Source")) {
                if (current != selectedObject && current->GetParent() != selectedObject && selectedObject->GetParent() != current) {
                    App->scene->SetNewParent(selectedObject, current);
                    //current->setParent(selectedObject);
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::BeginPopup("GameObject", ImGuiWindowFlags_AlwaysAutoResize))
        {
            //static char createName[64] = "New GameObject";
            //ImGui::InputText("Name", createName, 64);
            if (ImGui::Button("Create empty GameObject")) {
                App->scene->CreateGameObject(current, "New GameObject", false);
            }
            if (ImGui::Button("Delete GameObject")) {
                App->scene->RemoveGameObject(current);
            }
            if (ImGui::Button("Cancel"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        for (int i = 0; i < current->children.size(); ++i) {
            ImGui::PushID(current->children[i]);
            GetHierarchy(current->children[i]);
            ImGui::PopID();
        }
        ImGui::TreePop();
        
    }
}

void ModuleEditor::Properties() {

    //gameobjects = App->scene->GetGameObjects();
    static bool GoActive = true;

    ImGui::Begin("Inspector", &show_app_prop);
    
    if (selectedObject != nullptr && !selectedObject->GetRoot()) {
        if (ImGui::CollapsingHeader("General", &open_transformation, ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Name: %s", selectedObject->name);
            /*static char name[64];
            //strcpy_s(name, 64, gameobjects[0]->name);
            
            if (ImGui::InputText("Name", name, 64,
                ImGuiInputTextFlags_AutoSelectAll))
                gameobjects[0]->name = name;
            //static char name[64];

            //for (int i = 0; selectedObject->name[i] != '\0'; ++i) {
            //    name[i] = selectedObject->name[i];
            //}

            //ImGui::InputText("Name", name, 64);
            //if (ImGui::Button("Change Name"))
            //    selectedObject->SetName(name);
            /*
            if (ImGui::BeginPopupModal("Remove", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Remove Gameobject?");
                if (ImGui::Selectable("Yes")) {
                    App->scene->RemoveGameObject(selectedObject);
                }
                if (ImGui::Button("No"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }*/
        }
    }
    

    // Transformation
    if (ImGui::CollapsingHeader("Transformation", &open_transformation, ImGuiTreeNodeFlags_DefaultOpen)) {
        //Coords
        ImGui::Text("Transform");
        ImGui::InputFloat3("Position", position, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Rotation", rotation, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Scale", scale, "%.3f", ImGuiInputTextFlags_ReadOnly);
    }
    if (ImGui::CollapsingHeader("Mesh", &open_camera, ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Mesh");
        ImGui::SameLine();
        ImGui::Button(meshName);
        if (ImGui::BeginPopupContextItem())
        {
            ImGui::Text("Meshes:");
            if (ImGui::Selectable("BakerHouse")) { meshName = "BakerHouse"; }
            if (ImGui::Selectable("AnotherOne")) { meshName = "AnotherOneMesh"; }//All of this is temporal            
            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
    }
    
    if (ImGui::CollapsingHeader("Material", &open_camera, ImGuiTreeNodeFlags_DefaultOpen))
    {

    }

    // Geometry
    if (ImGui::CollapsingHeader("Geometry", &open_geometry, ImGuiTreeNodeFlags_DefaultOpen)) {

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
    if (ImGui::CollapsingHeader("Texture", &open_texture, ImGuiTreeNodeFlags_DefaultOpen)) {
        for (int i = 0; i < App->model->GetNumMaterials(); ++i) {
            /*int textw = App->model->GetTextureWidth(i);
            int texth = App->model->GetTextureHeight(i);*/

            ImGui::Text("Texture %d size: %d %d", i+1, 200, 200);
            ImGui::Image((ImTextureID)App->model->GetMaterial(i), {200, 200});
        }

    }
    if (ImGui::CollapsingHeader("Camera", &open_camera, ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Front: %f %f %f", App->camera->getFront().x, App->camera->getFront().y, App->camera->getFront().z);
        ImGui::Text("Up: %f %f %f", App->camera->getUp().x, App->camera->getUp().y, App->camera->getUp().z);
        ImGui::Text("Right: %f %f %f", App->camera->getRight().x, App->camera->getRight().y, App->camera->getRight().z);
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
        ImGui::SliderFloat3("Ambient color", ambient_color, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light color", light_color, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light direction", light_direction, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular n", &nSpec, 0.5f, 250.0f);
        ImGui::SliderFloat("Kd", &kDiff, 0.1f, 2.0f);
        ImGui::SliderFloat("Ks", &kSpec, 0.04f, 1.0f);
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
    delete[]ambient_color;

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

//See assets folder, browser, etc
void ModuleEditor::Project()
{
    ImGui::Begin("Project");
    ImGui::End();
}



