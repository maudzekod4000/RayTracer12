#ifndef TYPE_DEFS_H
#define TYPE_DEFS_H

#include "glm/glm.hpp"

typedef glm::vec3 Color;
typedef glm::vec2 Point2D;

struct Ray {
  glm::vec3 origin; // Could be a reference
  glm::vec3 dir;
};

#endif // !TYPE_DEFS_H
