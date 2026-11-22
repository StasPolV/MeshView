#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader 
{
public:
	Shader(const std::string& vertex_shader, const std::string& fragment_shader, bool from_file);
	
	static Shader FromSource(const std::string& vertex_shader_src, const std::string& fragment_shader_src);
	static Shader FromFile(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

	void Use() const;

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;

	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) = delete;
private:
	unsigned int CompileShader(GLuint shader_type, const char* src);
	void LinkProgram(GLuint v_shader, GLuint f_shader);

	GLuint m_id;
};
