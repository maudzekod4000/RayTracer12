#ifndef TYPE_DEFS_H
#define TYPE_DEFS_H

#include <iostream>
#include <vector>
#include <cstdint>

#define GLM_FORCE_SSE2
#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_PRECISION_LOWP_FLOAT
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#define MAX_COLOR 255

typedef glm::vec2 Point2D;
typedef glm::vec3 Vec3;

/// Each color component is between 0-1
typedef glm::vec3 NormalizedColor;

#endif // !TYPE_DEFS_H
