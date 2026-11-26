#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <optional>

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coords;
};

class Mesh 
{
public:
	Mesh(const std::vector<Vertex>& vertices,
		const std::vector<unsigned int>& indices, std::optional<int> material_index = std::nullopt);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void UploadToGPU();
	void Draw() const;

	std::optional<int> GetMaterialIndex() const;

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;

	std::optional<int> m_material_index;
};
