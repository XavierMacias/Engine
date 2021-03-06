#include "Application.h"
#include "ModuleModel.h"
#include "ModuleTexture.h"
#include "Assimp/cimport.h"
#include "Assimp/postprocess.h"

ModuleModel::ModuleModel()
{

}

// Destructor
ModuleModel::~ModuleModel()
{
}

// Called before render is available
bool ModuleModel::Init()
{
	return true;
}


// Called every draw update
update_status ModuleModel::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleModel::CleanUp()
{
	for (unsigned i = 0; i < materials.size(); ++i) {
		App->texture->FreeTexture(materials[i]);
	}
	materials.clear();
	meshes.clear();
	return true;

}

void ModuleModel::Load(const char* file_name) {
	scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene) {
		LoadMaterials();
		LoadMeshes();
	}
	else {
		LOG("ERROR");
	}
}

void ModuleModel::LoadMaterials() {
	aiString file;
	//materials.reserve(scene->mNumMaterials);

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			materials.push_back(App->texture->Load(file.data));
		}
	}
}

void ModuleModel::LoadMeshes() {
	aiString file;
	//meshes.reserve(scene->mNumMeshes);

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* currentMesh = scene->mMeshes[i];
		Mesh mesh;
		mesh.LoadVBO(currentMesh);
		mesh.LoadEBO(currentMesh);
		mesh.CreateVAO();
		meshes.push_back(mesh);
	}
}

void ModuleModel::Draw() {
	for (unsigned i = 0; i < meshes.size(); ++i) {
		meshes[i].Draw(materials);
	}
}

int ModuleModel::GetVertices() {
	int vertices = 0;
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		vertices += scene->mMeshes[i]->mNumVertices;
	}

	return vertices;
}

int ModuleModel::GetTriangles() {

	return GetVertices() / 3;
}
