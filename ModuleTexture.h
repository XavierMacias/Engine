#pragma once
#include "Module.h"
#include "Globals.h"
#include "DevIL/include/IL/ilu.h"
#include <iostream>
#include <string>

class ModuleTexture : public Module
{
public:
	ModuleTexture();

	bool Init();
	update_status Update();
	bool CleanUp();
	unsigned Load(const char* file_name, const char* model_path);
	void FreeTexture(unsigned textID);
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	
private:
	int width, height;
};

