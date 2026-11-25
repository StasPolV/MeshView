#pragma once

#include <glm/glm.hpp>
#include "Light.h"

struct SpotLight final : public Light
{
	glm::vec3 position{ 0.0f };
	glm::vec3 direction{ 0.0f, 0.0f, -1.0f };

	float constant{ 1.0f };
	float linear{ 0.09f };
	float quadratic{ 0.032f };

	float cut_off{ 12.5f };
	float outer_cut_off{ 17.5f };
};
