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
	int GetVertices(int mesh) { return scene->mMeshes[mesh]->mNumVertices; }
	int GetMeshes() { return scene->mNumMeshes; }
	int GetFaces(int mesh) { return scene->mMeshes[mesh]->mNumFaces; }
	float GetScale();
	int GetNumMaterials() { return materials.size(); }
	unsigned GetMaterial(int mat) { return materials[mat]; }
	int GetTextureWidth();
	int GetTextureHeight();

private:
	void LoadMaterials();
	void LoadMeshes();
	const aiScene* scene;

	std::vector<unsigned> materials;
	std::vector<Mesh> meshes;
};

