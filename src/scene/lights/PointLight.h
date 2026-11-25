#pragma once

#include <glm/glm.hpp>
#include "Light.h"

struct PointLight final : public Light
{
	glm::vec3 position{ 0.0f };

	float constant{ 1.0f };
	float linear{ 0.09f };
	float quadratic{ 0.032f };
};
