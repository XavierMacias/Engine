#pragma once
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/float4.h"

#include <iostream>
#include <string>

#include "GL/glew.h"
#include <vector>
#include <string>
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	void LoadModel(char* filename);
	void* GetContext() { return context; }

	void CreateFrameBuffer();
	unsigned int loadCubemap(std::vector<std::string> faces);
	void VAOSkybox();

	//Framebuffer
	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	
	//Cubemap
	unsigned int textureID;
	std::vector<std::string> faces;
public:	
	float3 grid;
	float4 background;

private:
	std::string getFileExt(const std::string s);	
	void* context;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
};

