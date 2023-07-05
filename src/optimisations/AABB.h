#ifndef AABB_H
#define AABB_H

#include <vector>
#include <limits>

#include <utils/TypeDefs.h>
#include "sampling/Triangle.h"
#include "sampling/Vertex.h"

struct AABB {
  AABB() : min(Vec3(std::numeric_limits<float>::max())), max(-min) {}
  AABB(Vec3& min, Vec3& max) : min(min), max(max) {}
  Vec3 min;
  Vec3 max;

  inline void expand(const Triangle& triangle) {
    std::vector<Vertex> vertices = { triangle.a, triangle.b, triangle.c };

    for (Vertex& vertex : vertices) {
      min.x = glm::min(vertex.pos.x, min.x);
      min.y = glm::min(vertex.pos.y, min.y);
      min.z = glm::min(vertex.pos.z, min.z);

      max.x = glm::max(vertex.pos.x, max.x);
      max.y = glm::max(vertex.pos.y, max.y);
      max.z = glm::max(vertex.pos.z, max.z);
    }
  }

  inline bool intersect(const Triangle& triangle) const {
    AABB triBox;
    triBox.expand(triangle);
    return this->intersect(triBox);
  }

  inline bool intersect(const AABB& aabb) const {
    return max.x >= aabb.min.x && aabb.max.x >= min.x &&
      max.y >= aabb.min.y && aabb.max.y >= min.y &&
      max.z >= aabb.min.z && aabb.max.z >= min.z;
  }

  inline bool intersect(const Ray& r) const {
    float tmin = (min.x - r.origin.x) / r.dir.x;
    float tmax = (max.x - r.origin.x) / r.dir.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (min.y - r.origin.y) / r.dir.y;
    float tymax = (max.y - r.origin.y) / r.dir.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
      return false;

    if (tymin > tmin)
      tmin = tymin;

    if (tymax < tmax)
      tmax = tymax;

    float tzmin = (min.z - r.origin.z) / r.dir.z;
    float tzmax = (max.z - r.origin.z) / r.dir.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
      return false;

    if (tzmin > tmin)
      tmin = tzmin;

    if (tzmax < tmax)
      tmax = tzmax;

    return true;
  }
};

#endif // !AABB_H
