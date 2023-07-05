#ifndef TRACER_H
#define TRACER_H

#include "scene/Scene.h"
#include "sampling/Lighting.h"
#include "sampling/IntersectionData.h"
#include "sampling/Triangle.h"
#include "sampling/Object.h"
#include "optimisations/AABBTree.h"

struct Tracer {
  Tracer(Scene& scene, Lighting& lights, AABBTree& aabb) : scene(scene), lighting(lights), aabb(aabb) {}

  InternalColor trace(const Ray& ray, int depth) {
    IntersectionData intersectionData = aabb.intersectAABBTree(ray);

    if (intersectionData.intersection) {
      if (intersectionData.mat.type == MaterialType::DIFFUSE || intersectionData.mat.type == MaterialType::NONE) {
        return intersectionData.mat.albedo + lighting.light(intersectionData);
      }
      else if (intersectionData.mat.type == MaterialType::REFLECTIVE) {
        Ray newReflectionRay{
          intersectionData.p + (intersectionData.pN * reflectionBias),
          glm::reflect(ray.dir, intersectionData.pN)
        };
        return trace(newReflectionRay, depth + 1) + lighting.light(intersectionData);
      }
    }

    return scene.settings.backgroundColor;
  }
private:
  Scene& scene;
  Lighting& lighting;
  AABBTree& aabb;
  float reflectionBias = 0.01f;
};


#endif // !TRACER_H
