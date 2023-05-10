#ifndef TYPE_DEFS_H
#define TYPE_DEFS_H

#include <iostream>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

typedef glm::vec2 Point2D;
typedef glm::vec3 Vec3;

struct Ray {
  Vec3 origin;
  Vec3 dir;
  float minT = 999999.9f;
};

using namespace glm;

struct Color {
  inline Color(uint8_t r, uint8_t g, uint8_t b) : Color(Vec3(r, g, b)) {}
  inline Color(Vec3 vec) : r(vec.x), g(vec.y), b(vec.z) {}
  inline Color() {}
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Triangle {
  inline Triangle(const Vec3& a, const Vec3& b, const Vec3& c, const Color& col): a(a), b(b), c(c), col(
    col
  ) {}
  Vec3 a;
  Vec3 b;
  Vec3 c;
  Color col;

  inline float area() const {
    return glm::length(glm::cross(b - a, c - a)) / 2.0f;
  }

  /* Calculates and returns the normal. It is intentional that it is not stored as a field of the struct. */
  inline Vec3 normal() const {
    return glm::normalize(glm::cross(b - a, c - a));
  }

  inline bool intersect(Ray& ray) const {
    Vec3 n = normal();
    float rayProjectionOnPlaneNormal = dot(ray.dir, n);

    /* The ray is perpendicular or facing the back of the triangle */
    if (rayProjectionOnPlaneNormal > 0.0f) {
      return false;
    }

    Vec3 rayOriginToPointOnTriangleVec = this->a - ray.origin; \
      float distanceFromRayOriginToPlane = glm::dot(rayOriginToPointOnTriangleVec, n);

    float t = distanceFromRayOriginToPlane / rayProjectionOnPlaneNormal;

    if (t > ray.minT) {
      return false;
    }

    Vec3 p = ray.origin + t * ray.dir;
    float e = 0.0001f;

    if (dot(n, cross(b - a, p - a)) - e <= 0.0f ||
      dot(n, cross(c - b, p - b)) - e <= 0.0f ||
      dot(n, cross(a - c, p - c)) - e <= 0.0f) {
      return false;
    }

    ray.minT = t;

    return true;
  }
};

#endif // !TYPE_DEFS_H
