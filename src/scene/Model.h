#pragma once

#include "Mesh.h"
#include "Material.h"
#include "../renderer/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <vector>
#include <string>

class Model 
{
public:
	Model(const std::string& path, const Material& default_material);
	Model(Mesh mesh, std::shared_ptr<Material> material);

	void LoadModel(const std::string& path, const Material& default_material);
	void LoadMaterials(const aiScene* scene, const Material& default_material);

	void ProcessNode(const aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	std::shared_ptr<Material> ProcessMaterial(const aiMaterial* material, const Material& default_material);

	void Draw() const;

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	Model(Model&& other) noexcept;
	Model& operator=(Model&& other) noexcept;

	const std::vector<Mesh>& GetMeshes() const;
	const Material& GetMaterial(unsigned int index) const;

private:
	std::vector<Mesh> m_meshes;
	std::vector<std::shared_ptr<Material>> m_materials;

	std::string m_directory;
};
