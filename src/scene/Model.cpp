#include "Model.h"

#include <glm/glm.hpp>

#include <stdexcept>
#include <utility>

Model::Model(const std::string& path) 
{
	LoadModel(path);
}

void Model::LoadModel(const std::string& path) 
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) 
	{
		throw std::runtime_error(importer.GetErrorString());
	}
	m_meshes.clear();

	ProcessNode(scene->mRootNode, scene);
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

	// TODO: implement material parse

	return Mesh(std::move(vertices), std::move(indices));
}

void Model::Draw() const
{
	for (size_t i = 0; i < m_meshes.size(); ++i) 
	{
		m_meshes[i].Draw();
	}
}
