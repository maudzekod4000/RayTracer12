#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <utils/TypeDefs.h>

#include "sampling/Vertex.h"
#include "sampling/IntersectionData.h"

struct Triangle {
  inline Triangle(const Vertex& a, const Vertex& b, const Vertex& c) : a(a), b(b), c(c),
    n(glm::normalize(glm::cross(b.pos - a.pos, c.pos - a.pos))), objIdx(-1) {}

  Vertex a;
  Vertex b;
  Vertex c;
  Vec3 n;
  int objIdx;

  inline float area() const {
    return area(a.pos, b.pos, c.pos);
  }

  inline bool intersect(const Ray& ray, IntersectionData& intersectionData) const {
    float rayProjectionOnPlaneNormal = glm::dot(ray.dir, n);
    // This is the projection of the vector to the point in the triangle on the normal.
    // It is also the distance from the ray origin to the plane.
    float t = glm::dot(this->a.pos - ray.origin, n) / rayProjectionOnPlaneNormal;

    if (t > intersectionData.t || t <= 0) {
      return false;
    }

    Vec3 p = ray.origin + t * ray.dir;

    // If the intersection point is just a bit to the right of the triangle side we don't care.
    // We avoid artifacts when two triangle sides are next to each other.
    // We want them to blend a bit to avoid jagged edges.
    const float e = -0.000001;

    if (dot(n, cross(b.pos - a.pos, p - a.pos)) <= e ||
      dot(n, cross(c.pos - b.pos, p - b.pos)) <= e ||
      dot(n, cross(a.pos - c.pos, p - c.pos)) <= e) {
      return false;
    }

    intersectionData.t = t;
    intersectionData.p = p;
    intersectionData.intersection = true;

    // Calculate barycentric coordinates of the hit and set the hit normal
    float areaM = area(a.pos, p, c.pos);
    float areaN = area(a.pos, b.pos, p);
    float areaTri = area();

    float u = areaM / areaTri;
    float v = areaN / areaTri;
    float w = 1 - u - v;
    intersectionData.hitBaryCentricCoordinates = Vec3(1 - u - v, u, v);
    intersectionData.pN = b.smoothNormal * u + c.smoothNormal * v + a.smoothNormal * w;
    intersectionData.pNNonSmooth = n;

    return true;
  }

private:
  // Calculates the area of the triangle defined by the three point arguments.
  inline float area(const Vec3& a, const Vec3& b, const Vec3& c) const {
    return glm::length(glm::cross(b - a, c - a)) / 2.0f;
  }
};

#endif // !TRIANGLE_H
