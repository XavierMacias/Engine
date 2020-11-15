#pragma once
#include "Module.h"
#include "Globals.h"
#include "Mesh.h"
#include "Assimp/scene.h"
#include <vector>

class ModuleModel : public Module
{
public:
	ModuleModel();
	~ModuleModel();

	bool Init();
	update_status Update();
	bool CleanUp();
	void Load(const char* file_name);
	void Draw();

private:
	void LoadMaterials(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);

	std::vector<unsigned> materials;
	std::vector<Mesh> meshes;
};

