#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <vector>
#include <string>

class Model 
{
public:
	Model(const std::string& path);

	void LoadModel(const std::string& path);
	void ProcessNode(const aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	void Draw() const;

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

private:
	std::vector<Mesh> m_meshes;
};
