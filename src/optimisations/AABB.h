#ifndef AABB_H
#define AABB_H

#include <vector>
#include <limits>

#include <utils/TypeDefs.h>
#include "sampling/Triangle.h"
#include "sampling/Vertex.h"

float maxFloat = std::numeric_limits<float>::max();

struct AABB {
  AABB() : min(Vec3(maxFloat)), max(-min) {}
  Vec3 min;
  Vec3 max;

  void expand(const Triangle& triangle) {
    std::vector<Vertex> vertices = { triangle.a, triangle.b, triangle.c };

    for (Vertex& vertex : vertices) {
      if (vertex.pos.x < min.x && vertex.pos.y < min.y && vertex.pos.z < min.z) {
        min = vertex.pos;
      }
      if (vertex.pos.x > max.x && vertex.pos.y > max.y && vertex.pos.z > max.z) {
        max = vertex.pos;
      }
    }
  }
};

#endif // !AABB_H
