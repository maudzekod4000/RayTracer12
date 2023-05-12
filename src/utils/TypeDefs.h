#ifndef TYPE_DEFS_H
#define TYPE_DEFS_H

#include <iostream>
#include <vector>

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

struct Ray {
  Vec3 origin;
  Vec3 dir;
};

using namespace glm;

struct PPMColor {
  inline PPMColor(uint8_t r, uint8_t g, uint8_t b) :r(r), g(g), b(b) {}
  inline PPMColor(): r(0), g(0), b(0) {}
  uint8_t r;
  uint8_t g;
  uint8_t b;

  inline static PPMColor from(const InternalColor& color) {
    return PPMColor(static_cast<uint8_t>(color.r * MAX_COLOR), static_cast<uint8_t>(color.g * MAX_COLOR),
      static_cast<uint8_t>(color.b * MAX_COLOR));
  }
};


struct Light {
  Vec3 pos;
  float intensity;
};

struct LightOptions {
  float shadowBias;
  float albedo;
};

struct IntersectionData {
  Vec3 p;
  Vec3 pN;
  float t = 9999.9f;
  bool intersection;
};

struct Triangle {
  inline Triangle(const Vec3& a, const Vec3& b, const Vec3& c, const InternalColor& col): a(a), b(b), c(c), col(
    col
  ) {}
  Vec3 a;
  Vec3 b;
  Vec3 c;
  InternalColor col;

  inline float area() const {
    return glm::length(glm::cross(b - a, c - a)) / 2.0f;
  }

  /* Calculates and returns the normal. It is intentional that it is not stored as a field of the struct. */
  inline Vec3 normal() const {
    return glm::normalize(glm::cross(b - a, c - a));
  }

  inline bool intersect(Ray& ray, IntersectionData& intersectionData) const {
    Vec3 n = normal();
    float rayProjectionOnPlaneNormal = dot(ray.dir, n);

    /* The ray is perpendicular or facing the back of the triangle */
    /*if (rayProjectionOnPlaneNormal >= 0.0f) {
      return false;
    }*/

    Vec3 rayOriginToPointOnTriangleVec = this->a - ray.origin;
    // This is the projection of the vector to the point in the triangle on the normal.
    // It is also the distance from the ray origin to the plane.
    float distanceFromRayOriginToPlane = glm::dot(rayOriginToPointOnTriangleVec, n);
    float t = distanceFromRayOriginToPlane / rayProjectionOnPlaneNormal;

    if (t > intersectionData.t) {
      return false;
    }

    Vec3 p = ray.origin + t * ray.dir;
    float e = 0.0001f;

    if (dot(n, cross(b - a, p - a)) - e <= 0.0f ||
      dot(n, cross(c - b, p - b)) - e <= 0.0f ||
      dot(n, cross(a - c, p - c)) - e <= 0.0f) {
      return false;
    }

    intersectionData.t = t;
    intersectionData.p = p;
    intersectionData.intersection = true;
    intersectionData.pN = n;

    return true;
  }
};

struct Lighting {
  inline Lighting(LightOptions& opts, const std::vector<Light>& lights, const std::vector<Triangle>& triangles) :
    options(opts), lights(lights), triangles(triangles) {}

  inline InternalColor light(const IntersectionData& intersectionData) {
    InternalColor lightContributionColor{};

    for (const Light& light : lights) {
      Vec3 lightDir = light.pos - intersectionData.p;
      float lightSphereRadius = glm::length(lightDir);
      lightDir = glm::normalize(lightDir);
      float cosineLaw = glm::max(0.0f, glm::dot(lightDir, intersectionData.pN));
      float lightSphereArea = 4 * M_PI * lightSphereRadius * lightSphereRadius;
      Ray shadowRay{ intersectionData.p + intersectionData.pN * options.shadowBias, lightDir };

      bool intersects = false;
      IntersectionData intrData;

      for (const auto& triangle : triangles) {
        if (triangle.intersect(shadowRay, intrData)) {
          intersects = true;
          break;
        }
      }

      if (intersects) {
        float color = (light.intensity / (lightSphereArea * options.albedo * cosineLaw)) / (light.intensity / 2);
        lightContributionColor += InternalColor(color);
      }
    }

    return lightContributionColor;
  }

  LightOptions options;
  std::vector<Light> lights;
  std::vector<Triangle> triangles;
};

#endif // !TYPE_DEFS_H
