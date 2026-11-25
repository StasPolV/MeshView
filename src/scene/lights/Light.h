#pragma once

#include <glm/glm.hpp>

struct Light 
{
	glm::vec3 ambient{ 0.2f };
	glm::vec3 diffuse{ 0.8f };
	glm::vec3 specular{ 1.0f };
};
