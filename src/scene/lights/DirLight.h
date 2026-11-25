#pragma once

#include <glm/glm.hpp>
#include "Light.h"

struct DirLight final : public Light
{
	DirLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
		: Light(ambient, diffuse, specular), direction(direction)
	{

	}

	DirLight(const glm::vec3& direction)
		: Light(), direction(direction)
	{

	}

	glm::vec3 direction{ -0.2f, -1.0f, -0.3f };
};
