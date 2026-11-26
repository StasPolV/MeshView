#pragma once

#include <glad/glad.h>
#include "Shader.h"
#include "Texture.h"
#include "../scene/Mesh.h"
#include "../scene/lights/Light.h"
#include "../scene/lights/DirLight.h"
#include "../scene/lights/SpotLight.h"
#include "../scene/lights/PointLight.h"
#include "../scene/Material.h"

#include <memory>
#include <vector>

class Camera;

class Renderer 
{
public:
	void Initialize();
	void Render(const Camera* camera, float aspect_ratio);
	void Shutdown();

private:
	void BindMaterial();

	std::unique_ptr<Shader> m_model_shader;
	std::unique_ptr<Shader> m_light_shader;

	std::unique_ptr<Mesh> m_mesh;
	std::vector<std::shared_ptr<Texture>> m_textures;

	DirLight m_dir_light;
	std::vector<PointLight> m_point_lights;
	std::vector<SpotLight> m_spot_lights;

	std::vector<Material> m_materials;
};
