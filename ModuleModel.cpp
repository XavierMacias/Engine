#include "Application.h"
#include "ModuleModel.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"
#include "Assimp/cimport.h"
#include "Assimp/postprocess.h"
#include "Assimp/Importer.hpp"
#include "Assimp/aabb.h"
#include "MathGeoLib/Geometry/AABB.h"
#include "MathGeoLib/Geometry/Sphere.h"

ModuleModel::ModuleModel()
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
	scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes);
	if (scene) {
		LoadMaterials(file_name);
		LoadMeshes();
	}
	else {
		LOG("ERROR");
	}
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
			materials.push_back(App->texture->Load("default.png", model_path)); // put a black texture to avoid the engine crashes
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

void ModuleModel::LoadMeshes(const char* file_name) 
{
	const aiScene* sce;
	sce = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes);
	if (sce)
	{
		for (unsigned i = 0; i < sce->mNumMeshes; ++i)
		{
			aiMesh* currentMesh = sce->mMeshes[i];
			Mesh mesh;
			mesh.LoadVBO(currentMesh);
			mesh.LoadEBO(currentMesh);
			mesh.CreateVAO();
			meshes.push_back(mesh);
		}
	}	
}

float ModuleModel::ComputeCenter() {
	AABB aabb = AABB();
	float xmax, xmin, ymax, ymin, zmax, zmin;
	xmax = xmin = ymax = ymin = zmax = zmin = 0;

	xmax = scene->mMeshes[0]->mAABB.mMax.x;
	xmin = scene->mMeshes[0]->mAABB.mMin.x;
	ymax = scene->mMeshes[0]->mAABB.mMax.y;
	ymin = scene->mMeshes[0]->mAABB.mMin.y;
	zmax = scene->mMeshes[0]->mAABB.mMax.z;
	zmin = scene->mMeshes[0]->mAABB.mMin.z;

	for (unsigned i = 1; i < meshes.size(); ++i) {
		if (xmax < scene->mMeshes[i]->mAABB.mMax.x) xmax = scene->mMeshes[i]->mAABB.mMax.x;
		if (ymax < scene->mMeshes[i]->mAABB.mMax.y) ymax = scene->mMeshes[i]->mAABB.mMax.y;
		if (zmax < scene->mMeshes[i]->mAABB.mMax.z) zmax = scene->mMeshes[i]->mAABB.mMax.z;
		if (xmin > scene->mMeshes[i]->mAABB.mMin.x) xmin = scene->mMeshes[i]->mAABB.mMin.x;
		if (ymin > scene->mMeshes[i]->mAABB.mMin.y) ymin = scene->mMeshes[i]->mAABB.mMin.y;
		if (zmin > scene->mMeshes[i]->mAABB.mMin.z) zmin = scene->mMeshes[i]->mAABB.mMin.z;
	}

	aabb.maxPoint = vec(xmax,ymax,zmax);
	aabb.minPoint = vec(xmin, ymin, zmin);

	vec center = aabb.CenterPoint();

	float bd = (aabb.MinimalEnclosingSphere()).Diameter() / (float)2.0f;
	return bd;
}

void ModuleModel::Draw() 
{
	for (unsigned i = 0; i < meshes.size(); ++i) 
	{
		meshes[i].Draw(materials);
	}
}
