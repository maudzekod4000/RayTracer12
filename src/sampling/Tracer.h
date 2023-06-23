#ifndef TRACER_H
#define TRACER_H

#include "scene/Scene.h"
#include "sampling/Lighting.h"
#include "sampling/IntersectionData.h"
#include "sampling/Triangle.h"
#include "sampling/Object.h"
#include "optimisations/AABB.h"

struct Tracer {
  Tracer(Scene& scene, Lighting& lights, AABB& aabb) : scene(scene), lighting(lights), aabb(aabb) {}

  InternalColor trace(const Ray& ray, int depth) {
    IntersectionData intersectionData{};

    for (Object& obj : scene.objects) {
      for (Triangle& tr : obj.triangles) {
        if (tr.intersect(ray, intersectionData)) {
          intersectionData.mat = obj.mat;
        }
      }
    }

    if (intersectionData.intersection) {
      if (intersectionData.mat.type == "diffuse") {
        return intersectionData.mat.albedo + lighting.light(intersectionData);
      }
      else if (intersectionData.mat.type == "reflective") {
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
  AABB& aabb;
  float reflectionBias = 0.01f;
};


#endif // !TRACER_H
