#ifndef TYPE_DEFS_H
#define TYPE_DEFS_H

#include "glm/glm.hpp"

typedef glm::vec3 Color;
typedef glm::vec2 Point2D;
typedef glm::vec3 Vec3;

struct Ray {
  Vec3 origin; // Could be a reference
  Vec3 dir;
};

struct Triangle {
  inline Triangle(const Vec3& a, const Vec3& b, const Vec3& c): a(a), b(b), c(c) {
    normal = calculateNormal();
  }
  Vec3 a;
  Vec3 b;
  Vec3 c;
  Vec3 normal;

  inline float area() {
    return glm::length(glm::cross(b - a, c - a)) / 2.0f;
  }
private:
  inline Vec3 calculateNormal() {
    return glm::normalize(glm::cross(b - a, c - a));
  }
};

#endif // !TYPE_DEFS_H
