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

	bool Init();
	update_status Update();
	bool CleanUp();
	void Load(const char* file_name);
	void LoadTexture(const char* file_name);
	void Draw();
	int GetVertices(int mesh) { return scene->mMeshes[mesh]->mNumVertices; }
	int GetMeshes() { return scene->mNumMeshes; }
	int GetFaces(int mesh) { return scene->mMeshes[mesh]->mNumFaces; }
	int GetNumMaterials() { return materials.size(); }
	unsigned GetMaterial(int mat) { return materials[mat]; }
	int GetTextureWidth(int i) { return widths[i]; }
	int GetTextureHeight(int i) { return heights[i]; }
	float ComputeCenter();

public:
	void LoadMaterials(const char* model_path);
	void LoadMeshes();
	void LoadMeshes(const char* file_name);
	const aiScene* scene;
	//float boundaryRadius;

	std::vector<unsigned> materials;
	std::vector<Mesh> meshes;
	std::vector<int> widths;
	std::vector<int> heights;
};

