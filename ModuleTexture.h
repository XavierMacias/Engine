#pragma once
#include "Module.h"
#include "Globals.h"
#include "DevIL/include/IL/ilu.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status Update();
	bool CleanUp();
	unsigned Load(const char* file_name);
	void FreeTexture(unsigned textID);
};

