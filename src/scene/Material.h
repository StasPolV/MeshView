#pragma once

#include <memory>

class Texture;

struct Material 
{
	std::shared_ptr<Texture> diffuse_map;
	std::shared_ptr<Texture> specular_map;

	float shininess;
};
