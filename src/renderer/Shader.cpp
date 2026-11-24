#include <glad/glad.h>
#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace 
{
	std::string readFile(const std::string& file_path) 
	{
		std::ifstream file(file_path);
		if (!file.is_open())
		{
			std::cerr << "Failed to open file at " << file_path << '\n';
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		if (file.bad())
		{
			std::cerr << "Failed to read file at " << file_path << '\n';
		}

		return buffer.str();
	}
}

Shader::Shader(const std::string& vertex_shader, const std::string& fragment_shader, bool from_file) 
{
	std::string vertex_shader_src = vertex_shader;
	std::string fragment_shader_src = fragment_shader;
	if (from_file) 
	{
		vertex_shader_src = readFile(vertex_shader);
		fragment_shader_src = readFile(fragment_shader);
	}

	const char* vertex_src = vertex_shader_src.c_str();
	const char* fragment_src = fragment_shader_src.c_str();

	GLuint v_shader = CompileShader(GL_VERTEX_SHADER, vertex_src);
	GLuint f_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_src);

	LinkProgram(v_shader, f_shader);

	glDeleteShader(v_shader);
	glDeleteShader(f_shader);
}

Shader::~Shader() 
{
	glDeleteProgram(m_id);
}

Shader Shader::FromSource(const std::string& vertex_shader_src, const std::string& fragment_shader_src)
{
	return Shader(vertex_shader_src, fragment_shader_src, false);
}

Shader Shader::FromFile(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	return Shader(vertex_shader_path, fragment_shader_path, true);
}

void Shader::Use() const 
{
	glUseProgram(m_id);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
}

unsigned int Shader::CompileShader(GLuint shader_type, const char* src) 
{
	unsigned int shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	GLint vertex_compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &vertex_compiled);
	if (vertex_compiled != GL_TRUE) 
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(shader, 1024, &log_length, message);

		if (shader_type == GL_VERTEX_SHADER) 
		{
			std::cerr << "Failed to compile vertex shader!\n";
		}
		else if (shader_type == GL_FRAGMENT_SHADER) 
		{
			std::cerr << "Failed to compile fragment shader!\n";
		}
	}

	return shader;
}

void Shader::LinkProgram(GLuint v_shader, GLuint f_shader) 
{
	m_id = glCreateProgram();
	glAttachShader(m_id, v_shader);
	glAttachShader(m_id, f_shader);

	glLinkProgram(m_id);
	GLint program_linked;
	glGetProgramiv(m_id, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE) 
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(m_id, 1024, &log_length, message);

		std::cerr << "Failed to link program!\n";
	}
}
