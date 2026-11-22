#include "Mesh.h"

#include <utility>
#include <vector>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	: m_vertices(vertices), m_indices(indices)
{
	UploadToGPU();
}

void Mesh::UploadToGPU() 
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coords)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::Draw() const 
{
	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);

	glBindVertexArray(0);
}

Mesh::Mesh(Mesh&& other) noexcept 
{
	m_vertices = std::move(other.m_vertices);
	m_indices = std::move(other.m_indices);
	m_VAO = std::exchange(other.m_VAO, 0);
	m_VBO = std::exchange(other.m_VBO, 0);
	m_EBO = std::exchange(other.m_EBO, 0);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept 
{
	if (this != &other)
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);

		m_vertices = std::move(other.m_vertices);
		m_indices = std::move(other.m_indices);

		m_VAO = std::exchange(other.m_VAO, 0);
		m_VBO = std::exchange(other.m_VBO, 0);
		m_EBO = std::exchange(other.m_EBO, 0);
	}

	return *this;
}
