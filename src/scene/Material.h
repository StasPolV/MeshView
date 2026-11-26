#pragma once

#include <memory>

class Texture;

enum class MaterialType : int // TODO: create MaterialManager
{
    DarkWood,
    Gold,
    Marble
};

struct Material 
{
	std::shared_ptr<Texture> diffuse_map;
	std::shared_ptr<Texture> specular_map;

	float shininess;
};
