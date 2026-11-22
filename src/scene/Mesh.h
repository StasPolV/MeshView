#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

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
		const std::vector<unsigned int>& indices);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void UploadToGPU();
	void Draw() const;

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
};
