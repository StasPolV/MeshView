#include "Model.h"

#include <glm/glm.hpp>

#include <stdexcept>
#include <utility>
#include <filesystem>

Model::Model(const std::string& path, const Material& default_material)
{
	LoadModel(path, default_material);
}

Model::Model(Mesh mesh, std::shared_ptr<Material> material) 
{
	m_meshes.push_back(std::move(mesh));
	m_materials.push_back(material);
}

void Model::LoadModel(const std::string& path, const Material& default_material)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);

	if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) 
	{
		throw std::runtime_error(importer.GetErrorString());
	}
	m_meshes.clear();
	m_materials.clear();

	m_directory = std::filesystem::absolute(path).parent_path().string();

	LoadMaterials(scene, default_material);

	ProcessNode(scene->mRootNode, scene);
}

void Model::LoadMaterials(const aiScene* scene, const Material& default_material) 
{
	m_materials.reserve(scene->mNumMaterials);

	for (int i = 0; i < scene->mNumMaterials; ++i) 
	{
		m_materials.push_back(ProcessMaterial(scene->mMaterials[i], default_material));
	}
}

std::shared_ptr<Material> Model::ProcessMaterial(const aiMaterial* material, const Material& default_material) 
{
	auto result = std::make_shared<Material>(default_material);

	aiString texture_path;

	// Diffuse map
	if (material->GetTexture(
		aiTextureType_DIFFUSE, 0, &texture_path
	) == AI_SUCCESS)
	{
		std::string full_path = (
			std::filesystem::path(m_directory) /
			texture_path.C_Str()
			).lexically_normal().string();

		result->diffuse_map = std::make_shared<Texture>(
			full_path,
			GL_REPEAT,
			GL_REPEAT,
			GL_NEAREST,
			GL_NEAREST_MIPMAP_NEAREST
		);
	}

	// Specular map
	if (material->GetTexture(
		aiTextureType_SPECULAR, 0, &texture_path
	) == AI_SUCCESS)
	{
		std::string full_path = (
			std::filesystem::path(m_directory) /
			texture_path.C_Str()
			).lexically_normal().string();

		result->specular_map = std::make_shared<Texture>(
			full_path,
			GL_REPEAT,
			GL_REPEAT,
			GL_NEAREST,
			GL_NEAREST_MIPMAP_NEAREST
		);
	}

	// Shininess
	ai_real shininess;

	if (material->Get(
		AI_MATKEY_SHININESS, shininess
	) == AI_SUCCESS)
	{
		result->shininess = static_cast<float>(shininess);
	}

	return result;
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene) 
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) 
	{
		unsigned int mesh_index = node->mMeshes[i];

		const aiMesh* mesh = scene->mMeshes[mesh_index];

		m_meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) 
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene) 
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) 
	{
		Vertex vertex{};

		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->HasNormals()) 
		{
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		if (mesh->HasTextureCoords(0)) 
		{
			vertex.tex_coords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) 
	{
		const aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) 
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(std::move(vertices), std::move(indices), mesh->mMaterialIndex);
}

void Model::Draw() const
{
	for (size_t i = 0; i < m_meshes.size(); ++i) 
	{
		m_meshes[i].Draw();
	}
}

Model::Model(Model&& other) noexcept
	: m_meshes(std::move(other.m_meshes)), m_materials(std::move(other.m_materials)), m_directory(std::move(other.m_directory))
{

}

Model& Model::operator=(Model&& other) noexcept
{
	if (this != &other) 
	{
		m_meshes = std::move(other.m_meshes);
		m_materials = std::move(other.m_materials);
		m_directory = std::exchange(other.m_directory, "");
	}

	return *this;
}

const std::vector<Mesh>& Model::GetMeshes() const
{
	return m_meshes;
}

const Material& Model::GetMaterial(unsigned int index) const
{
	return *m_materials.at(index);
}
