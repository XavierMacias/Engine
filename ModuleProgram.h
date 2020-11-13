#pragma once
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status Update();
	bool CleanUp();
	char* LoadShaderSource(const char* name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned GetProgram() { return program_id;}
private:
	unsigned program_id, vtx_shader, frg_shader;
};

