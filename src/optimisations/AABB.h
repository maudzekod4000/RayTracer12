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

  inline void expand(const Triangle& triangle) {
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

  inline bool intersect(const Ray& ray) const {
    // First, calculate the t values for the intersection of the ray with all planes
    // with which the cube is defined.
    
    // Minimum point's x component t value
    float tminx = (min.x - ray.origin.x) / ray.dir.x;
    float tminy = (min.y - ray.origin.y) / ray.dir.y;
    float tminz = (min.z - ray.origin.z) / ray.dir.z;

    // Maximum component defined planes
    float tmaxx = (max.x - ray.origin.x) / ray.dir.x;
    float tmaxy = (max.y - ray.origin.y) / ray.dir.y;
    float tmaxz = (max.z - ray.origin.z) / ray.dir.z;

    // We need the 'greater' point between the intersections with the min-defined planes.
    // Greater, in 2D, means more to the upper right direction.
    float tmaxOfMin = tminx > tminy ? tminx : tminy;
    tmaxOfMin = tminz > tmaxOfMin ? tminz : tmaxOfMin;

    // Now for the maximum component we need the smaller point
    float tminOfMax = tmaxx < tmaxy ? tmaxx : tmaxy;
    tminOfMax = tmaxz < tminOfMax ? tmaxz : tminOfMax;

    if (tminOfMax < tmaxOfMin) {
      return false;
    }

    return true;
  }
};

#endif // !AABB_H
