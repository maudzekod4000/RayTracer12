#ifndef TRACER_H
#define TRACER_H

#include <math.h>

#include "scene/Scene.h"
#include "sampling/Lighting.h"
#include "sampling/IntersectionData.h"
#include "sampling/Triangle.h"
#include "sampling/Object.h"
#include "optimisations/AABBTree.h"

struct Tracer {
  Tracer(Scene& scene, Lighting& lights, AABBTree& aabb) : scene(scene), lighting(lights), aabb(aabb) {}

  inline NormalizedColor trace(const Ray& ray, int depth) {
    if (depth > 5) {
      return Vec3();
    }
    IntersectionData intersectionData = aabb.intersectAABBTree(ray);

    if (intersectionData.intersection) {
      if (intersectionData.mat.type == MaterialType::DIFFUSE || intersectionData.mat.type == MaterialType::NONE) {
        NormalizedColor clr = lighting.light(intersectionData);
        return intersectionData.mat.albedo + clr;
      }
      else if (intersectionData.mat.type == MaterialType::REFLECTIVE) {
        Ray newReflectionRay{
          intersectionData.p + (intersectionData.pN * reflectionBias),
          glm::reflect(ray.dir, intersectionData.pN)
        };
        return trace(newReflectionRay, depth + 1) + lighting.light(intersectionData);
      }
      else if (intersectionData.mat.type == MaterialType::REFRACTIVE) {
        // TODO: This does not work...
        Vec3 N = intersectionData.pN;
        Vec3 I = glm::normalize(ray.dir);
        float n1 = 1.0f;
        float n2 = 1.5f;
        if (glm::dot(I, N) > 0) {
          N = -N;
          std::swap(n1, n2);
        }
        float kr = 0.0f;
        fresnel(I, N, n2, kr);

        /*float cosin = -glm::dot(I, N); */
        NormalizedColor refrClr = Vec3(0.0f);
        if (kr < 1) {
          Vec3 refractVec = glm::refract(glm::normalize(ray.dir), intersectionData.pN, (1.0f / 1.5f));
          Ray refRay{
            intersectionData.p + (-N * reflectionBias),
            refractVec
          };
          refrClr = trace(refRay, depth + 1);
        }
        Ray reflRay{
          intersectionData.p + (N * reflectionBias),
          glm::reflect(I, N)
        };
        NormalizedColor reflClr = trace(reflRay, depth + 1);
        return kr * reflClr + (1 - kr) * refrClr;
      }
    }

    return scene.settings.backgroundColor;
  }
private:
  Scene& scene;
  Lighting& lighting;
  AABBTree& aabb;
  float reflectionBias = 0.01f;

  void fresnel(const Vec3& I, const Vec3& N, const float& ior, float& kr)
  {
    float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(I, N));
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
      kr = 1;
    }
    else {
      float cost = sqrtf(std::max(0.f, 1 - sint * sint));
      cosi = fabsf(cosi);
      float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
      float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
      kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, the transmittance is given by:
    // kt = 1 - kr;
  }
};


#endif // !TRACER_H
