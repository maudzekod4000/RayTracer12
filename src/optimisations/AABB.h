#ifndef AABB_H
#define AABB_H

#include <vector>
#include <limits>

#include "utils/TypeDefs.h"
#include "sampling/Triangle.h"
#include "sampling/Vertex.h"

struct AABB {
  explicit AABB(): min(Vec3(std::numeric_limits<float>::max())), max(-min) {}
  explicit AABB(Vec3& min, Vec3& max): min(min), max(max) {}
  explicit AABB(const Triangle& t): AABB() { this->expand(t); }
  Vec3 min;
  Vec3 max;

  inline void expand(const Triangle& triangle) {
    min = glm::min(triangle.a.pos, glm::min(triangle.b.pos, glm::min(triangle.c.pos, min)));
    max = glm::max(triangle.a.pos, glm::max(triangle.b.pos, glm::max(triangle.c.pos, max)));
  }

  inline bool intersect(const Triangle& triangle) const {
    return this->intersect(AABB(triangle));
  }

  inline bool intersect(const AABB& aabb) const {
    return max.x >= aabb.min.x && aabb.max.x >= min.x &&
      max.y >= aabb.min.y && aabb.max.y >= min.y &&
      max.z >= aabb.min.z && aabb.max.z >= min.z;
  }

  // Ray-box intersection algorithm taken from the internet
  inline bool intersect(const Ray& r) const {
    float tmin = (min.x - r.origin.x) / r.dir.x;
    float tmax = (max.x - r.origin.x) / r.dir.x;
    float temp = tmin;
    tmin = std::min(tmin, tmax);
    tmax = std::max(temp, tmax);

    float tymin = (min.y - r.origin.y) / r.dir.y;
    float tymax = (max.y - r.origin.y) / r.dir.y;

    float tempy = tymin;
    tymin = std::min(tymin, tymax);
    tymax = std::max(tempy, tymax);

    if ((tmin > tymax) || (tymin > tmax))
      return false;

    tmin = std::max(tmin, tymin);
    tmax = std::min(tymax, tmax);

    float tzmin = (min.z - r.origin.z) / r.dir.z;
    float tzmax = (max.z - r.origin.z) / r.dir.z;
    float tempz = tzmin;
    tzmin = std::min(tzmin, tzmax);
    tzmax = std::max(tempz, tzmax);

    return !((tmin > tzmax) || (tzmin > tmax));
  }
};

#endif // !AABB_H
