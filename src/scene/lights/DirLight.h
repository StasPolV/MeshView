#pragma once

#include <glm/glm.hpp>
#include "Light.h"

struct DirLight final : public Light
{
	glm::vec3 direction{ -0.2f, -1.0f, -0.3f };
};
