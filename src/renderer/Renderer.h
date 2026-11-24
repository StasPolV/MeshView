#pragma once

#include <glad/glad.h>
#include "Shader.h"

#include <memory>

class Camera;
class Mesh;

class Renderer 
{
public:
	void Initialize();
	void Render(const Camera& camera, float aspect_ratio);
	void Shutdown();

private:
	std::unique_ptr<Shader> m_model_shader;
	std::unique_ptr<Mesh> m_mesh;
};
