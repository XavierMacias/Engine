#include "Application.h"
#include "ModuleModel.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"
#include "Assimp/cimport.h"
#include "Assimp/postprocess.h"
#include "Assimp/Importer.hpp"

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
	scene = nullptr;
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
	for (unsigned i = 0; i < meshes.size(); ++i) {
		meshes[i].Free();
	}
	materials.clear();
	meshes.clear();
	widths.clear();
	heights.clear();
	return true;

}

void ModuleModel::Load(const char* file_name) {
	scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene) {
		LoadMaterials(file_name);
		LoadMeshes();
	}
	else {
		LOG("ERROR");
	}
}

float ModuleModel::GetScale() {
	double factor = 1.0;
	aiMetadata* meta = scene->mMetaData;
	LOG("FACTOR: %f", factor);
	return factor;
}

void ModuleModel::LoadMaterials(const char* model_path) {
	aiString file;

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			materials.push_back(App->texture->Load(file.data, model_path));
			App->editor->AddLog("Texture loaded\n");
		}
		else {
			App->editor->AddLog("Difusse texture not found...\n");
			materials.push_back(App->texture->Load("black.png", model_path)); // put a black texture to avoid the engine crashes
		}
		widths.push_back(App->texture->GetWidth());
		heights.push_back(App->texture->GetHeight());
	}
}

void ModuleModel::LoadTexture(const char* file) {
	materials.push_back(App->texture->Load(file, file));
	widths.push_back(App->texture->GetWidth());
	heights.push_back(App->texture->GetHeight());
}

void ModuleModel::LoadMeshes() {
	aiString file;

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
