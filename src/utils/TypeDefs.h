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

using namespace glm;

struct Triangle {
  inline Triangle(const Vec3& a, const Vec3& b, const Vec3& c): a(a), b(b), c(c), col(
    Color(rand() % 255, rand() % 255, rand() % 255)
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

  inline bool intersect(const Ray& ray) const {
    Vec3 n = normal();
    float rayPlane = dot(ray.dir, n);

    /* The ray is perpendicular or facing the back of the triangle */
    if (rayPlane > 0.0f) {
      return false;
    }

    float t = glm::dot(this->a - ray.origin, n) / rayPlane;
    Vec3 p = ray.origin + t * ray.dir;
    
    float e = 0.000000001f;

    return (dot(n, cross(b - a, p - a)) - e > 0.0f) &&
      (dot(n, cross(c - b, p - b)) - e > 0.0f) &&
      (dot(n, cross(a - c, p - c)) - e > 0.0f);
  }
};

#endif // !TYPE_DEFS_H
