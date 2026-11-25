#pragma once

#include <glad/glad.h>

#include <string>

class Texture 
{
public:
	Texture(const std::string& path, GLint wrap_s, GLint wrap_t, GLint mag_filter, GLint min_filter);
	~Texture();

	void Bind(GLuint slot = 0) const;
	void Unbind() const;

	void Load(const std::string& path);
	void SetUp(GLint wrap_s, GLint wrap_t, GLint mag_filter, GLint min_filter);

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

private:
	GLuint m_texture;

	int m_width;
	int m_height;
	int m_channels;
};
