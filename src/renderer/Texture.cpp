#include "Texture.h"

#include <iostream>
#include <utility>

#include <stb_image.h>

Texture::Texture(const std::string& path, GLint wrap_s, GLint wrap_t, GLint mag_filter, GLint min_filter) 
{
	glGenTextures(1, &m_texture);
	Bind();
	SetUp(wrap_s, wrap_t, mag_filter, min_filter);
	Load(path);
}

void Texture::Bind(GLuint slot) const 
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::Unbind() const 
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Load(const std::string& path) 
{
	unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
	if (data) 
	{
		GLenum format;
		switch (m_channels) 
		{
		case 1:
			format = GL_RED;
			break;
		case 2:
			format = GL_RG;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cerr << "Can't load texture from " << path << '\n';
	}
	stbi_image_free(data);
}

void Texture::SetUp(GLint wrap_s, GLint wrap_t, GLint mag_filter, GLint min_filter) 
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

Texture::~Texture() 
{
	glDeleteTextures(1, &m_texture);
}

Texture::Texture(Texture&& other)
	: m_texture(std::exchange(other.m_texture, 0)), m_width(std::exchange(other.m_width, 0)),
	  m_height(std::exchange(other.m_height, 0)), m_channels(std::exchange(other.m_channels, 0))
{

}

Texture& Texture::operator=(Texture&& other) 
{
	if (this != &other) 
	{
		glDeleteTextures(1, &m_texture);
		m_texture = std::exchange(other.m_texture, 0);
		m_width = std::exchange(other.m_width, 0);
		m_height = std::exchange(other.m_height, 0);
		m_channels = std::exchange(other.m_channels, 0);
	}
	return *this;
}
