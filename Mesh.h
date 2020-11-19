#pragma once
#include "Assimp/mesh.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& model_textures);
	void Free();
private:
	unsigned vbo, ebo, vao;
	int material_index, num_vertices, num_indices;
};

