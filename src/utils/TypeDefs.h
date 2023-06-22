#ifndef TYPE_DEFS_H
#define TYPE_DEFS_H

#include <iostream>
#include <vector>
#include <cstdint>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define MAX_COLOR 255

typedef glm::vec2 Point2D;
typedef glm::vec3 Vec3;

/// Each color component is between 0-1
typedef glm::vec3 InternalColor;


#endif // !TYPE_DEFS_H
