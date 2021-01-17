#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleScene.h"
#include "ModuleModel.h"
#include "ModuleDebugDraw.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/float3x3.h"
#include "assimp/cimport.h"
#include "Gizmos/ImGuizmo.h"

ModuleRenderExercise::ModuleRenderExercise()	
{

}

void myCallback(const char* msg, char* userData) {
	App->editor->AddLog(msg);
}

// Called before render is available
bool ModuleRenderExercise::Init()
{

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	context = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();


	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
	
	CreateFrameBuffer();

	VAOSkybox();
	/*App->program->ShaderInit(".\\Shaders\\VertexShaderSky.glsl");
	App->program->ShaderInit(".\\Shaders\\FragmentShaderSky.glsl");*/

	faces =
	{
			"\skybox\right.jpg",
			"\skybox\left.jpg",
			"\skybox\top.jpg",
			"\skybox\bottom.jpg",
			"\skybox\front.jpg",
			"\skybox\back.jpg"
	};
		

	cubemapTexture = loadCubemap(faces);

	return true;
	
}

bool ModuleRenderExercise::Start() {
	App->scene->AddObject("BakerHouse.fbx");
	return true;
}

void ModuleRenderExercise::LoadModel(char* filename) {
	if (getFileExt((std::string)filename) == "fbx" || getFileExt((std::string)filename) == "FBX") {
		App->scene->AddObject(filename);
	}
	//else if (getFileExt((std::string)filename) == "png" || getFileExt((std::string)filename) == "dds") {
	//	for (int i = 0; i < App->model->GetNumMaterials(); ++i) {
	//		App->texture->FreeTexture(App->model->GetMaterial(i));
	//	}
	//	App->model->LoadTexture(filename);
	//}
	else {
		App->editor->AddLog("This isn't a FBX file");
	}
}

std::string ModuleRenderExercise::getFileExt(const std::string s) {

	size_t i = s.rfind('.', s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}

update_status ModuleRenderExercise::PreUpdate()
{		
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{	
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	grid.x = App->editor->gridColor[0];
	grid.y = App->editor->gridColor[1];
	grid.z = App->editor->gridColor[2];

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glEnable(GL_DEPTH_TEST); 
	background.x = App->editor->bGround[0];
	background.y = App->editor->bGround[1];
	background.z = App->editor->bGround[2];
	background.w = App->editor->bGround[3];

	glClearColor(background[0], background[1], background[2], background[3]);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	App->draw->Draw(App->camera->setViewMatrix(), App->camera->setProjectionMatrix(), w, h, grid);

	//App->model->Draw();
	for (int i = 0; i < App->scene->GetGameObjects().size(); ++i) {
		GameObject* go = App->scene->GetGameObjects()[i];

		std::vector<Component*> meshComponents;
		go->GetComponent(Component::ComponentType::MESH_COMPONENT, meshComponents);
		std::vector<Component*> matComponents;
		go->GetComponent(Component::ComponentType::MATERIAL_COMPONENT, matComponents);
		std::vector<unsigned> materials;

		for (int i = 0; i < matComponents.size(); ++i) {
			MaterialComponent* mc = (MaterialComponent*)matComponents[i];
			materials.push_back(mc->GetMaterial());
		}

		for (unsigned i = 0; i < meshComponents.size(); ++i)
		{
			MeshComponent* mc = (MeshComponent*)meshComponents[i];
			mc->GetMesh().Draw(materials);
		}

	}

	App->program->ShaderInit(".\\Shaders\\VertexShader.glsl");
	App->program->ShaderInit(".\\Shaders\\FragmentShader.glsl");

	glDepthFunc(GL_LEQUAL); 	
	unsigned program = App->program->GetProgram();
	const float4x4& view = App->camera->setViewMatrix();
	const float4x4& proj = App->camera->setProjectionMatrix();
	glUseProgram(program);	
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderExercise::WindowResized(unsigned width, unsigned height)
{
	App->camera->SetFOV(float(width) / float(height));
}

void ModuleRenderExercise::CreateFrameBuffer()
{
	//Frame buffer object	
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//Texture color buffer	
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	//Render buffer object
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);				
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);	
																									
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int ModuleRenderExercise::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID = 0;
	/*glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	*/
	return textureID;
}

void ModuleRenderExercise::VAOSkybox()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	// skybox VAO	
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
