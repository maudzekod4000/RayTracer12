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
  int depth{};
};

using namespace glm;

struct PPMColor {
  inline PPMColor(uint8_t r, uint8_t g, uint8_t b) :r(r), g(g), b(b) {}
  inline PPMColor(): r(0), g(0), b(0) {}
  uint8_t r;
  uint8_t g;
  uint8_t b;

  inline static PPMColor from(const InternalColor& color) {
    float r = glm::clamp(color.r, 0.0f, 1.0f);
    float g = glm::clamp(color.g, 0.0f, 1.0f);
    float b = glm::clamp(color.b, 0.0f, 1.0f);
    /*float r = color.r;
    float g = color.g;
    float b = color.b;*/
    uint8_t red = static_cast<uint8_t>(r * MAX_COLOR);
    uint8_t green = static_cast<uint8_t>(g * MAX_COLOR);
    uint8_t blue = static_cast<uint8_t>(b * MAX_COLOR);
    return PPMColor(static_cast<uint8_t>(r * MAX_COLOR), static_cast<uint8_t>(g * MAX_COLOR),
      static_cast<uint8_t>(b * MAX_COLOR));
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
  Vec3 hitBaryCentricCoordinates;
  Vec3 pNNonSmooth;
  float t = 9999.9f;
  bool intersection;
};

struct Vertex {
  Vec3 pos{};
  Vec3 smoothNormal{};
};

struct Triangle {
  inline Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const InternalColor& col): a(a), b(b), c(c), col(
    col
  ) {}
  Vertex a;
  Vertex b;
  Vertex c;
  InternalColor col;

  inline float area() const {
    return glm::length(glm::cross(b.pos - a.pos, c.pos - a.pos)) / 2.0f;
  }

  /* Calculates and returns the normal. It is intentional that it is not stored as a field of the struct. */
  inline Vec3 normal() const {
    return glm::normalize(glm::cross(b.pos - a.pos, c.pos - a.pos));
  }

  inline bool intersect(Ray& ray, IntersectionData& intersectionData) const {
    Vec3 n = normal();

    float rayProjectionOnPlaneNormal = dot(ray.dir, n);

    /* The ray is perpendicular we have no work */
    if (rayProjectionOnPlaneNormal == 0.0f) {
      return false; 
    }

    Vec3 rayOriginToPointOnTriangleVec = this->a.pos - ray.origin;
    // This is the projection of the vector to the point in the triangle on the normal.
    // It is also the distance from the ray origin to the plane.
    float distanceFromRayOriginToPlane = glm::dot(rayOriginToPointOnTriangleVec, n);
    float t = distanceFromRayOriginToPlane / rayProjectionOnPlaneNormal;

    if (t > intersectionData.t || t <= 0) {
      return false;
    }

    Vec3 p = ray.origin + t * ray.dir;

    if (dot(n, cross(b.pos - a.pos, p - a.pos)) < 0.0f ||
      dot(n, cross(c.pos - b.pos, p - b.pos)) < 0.0f ||
      dot(n, cross(a.pos - c.pos, p - c.pos)) < 0.0f) {
      return false;
    }

    intersectionData.t = t;
    intersectionData.p = p;
    intersectionData.intersection = true;

    // Calculate barycentric coordinates of the hit and set the hit normal
    float areaM = glm::length(glm::cross(p - a.pos, c.pos - a.pos)) / 2;
    float areaN = glm::length(glm::cross(b.pos - a.pos, p - a.pos)) / 2;
    float areaTri = glm::length(glm::cross(b.pos - a.pos, c.pos - a.pos)) / 2;

    float u = areaM / areaTri;
    float v = areaN / areaTri;
    float w = 1 - u - v;
    intersectionData.hitBaryCentricCoordinates = Vec3(1 - u - v, u, v);
    intersectionData.pN = b.smoothNormal * u + c.smoothNormal * v + a.smoothNormal * w;
    intersectionData.pNNonSmooth = n;

    return true;
  }
};

struct Material {
  std::string type;
  Vec3 albedo;
  bool smoothShading;
  float ior{};
};

struct Object {
  std::vector<Triangle> triangles;
  Material mat;
};

struct Lighting {
  inline Lighting(LightOptions& opts, const std::vector<Light>& lights, const std::vector<Object>& objects) :
    options(opts), lights(lights), objects(objects) {}

  inline InternalColor light(const IntersectionData& intersectionData) {
    InternalColor lightContributionColor{};

    for (const Light& light : lights) {
      Vec3 lightDir = light.pos - intersectionData.p;
      float lightSphereRadius = glm::length(lightDir);
      lightDir = glm::normalize(lightDir);
      float angle = glm::dot(intersectionData.pN, lightDir);

      float cosineLaw = glm::max(0.0f, angle);
      float lightSphereArea = 4 * M_PI * lightSphereRadius * lightSphereRadius;
      Ray shadowRay{ intersectionData.p + (intersectionData.pN * options.shadowBias), lightDir};

      bool intersects = false;
      IntersectionData intrData{};

      for (Object& obj : objects) {
        for (const auto& triangle : obj.triangles) {
          if (triangle.intersect(shadowRay, intrData)) {
            if (intrData.t < lightSphereRadius) {
              intersects = true;
              break;
            }
          }
        }
      }

      if (!intersects) {
        float color = light.intensity / lightSphereArea * options.albedo * cosineLaw;
        lightContributionColor += InternalColor(color);
      }
    }

    return lightContributionColor;
  }

  LightOptions options;
  std::vector<Light> lights;
  std::vector<Object> objects;
};

#endif // !TYPE_DEFS_H
