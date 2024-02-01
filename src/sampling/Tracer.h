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
    if (depth > 10) {
      return Vec3();
    }
    IntersectionData intersectionData = aabb.intersectAABBTree(ray);

    if (intersectionData.intersection) {
      if (intersectionData.mat.type == MaterialType::DIFFUSE || intersectionData.mat.type == MaterialType::NONE) {
        NormalizedColor clr = lighting.light(intersectionData);
        return intersectionData.mat.albedo + clr;
      }
      else if (intersectionData.mat.type == MaterialType::REFLECTIVE) {
        bool outside = glm::dot(ray.dir, intersectionData.pN) < 0;
        Vec3 bias = reflectionBias * intersectionData.pN;
        Ray newReflectionRay(
          outside ? intersectionData.p + bias : intersectionData.p - bias,
          glm::reflect(ray.dir, intersectionData.pN)
        );
        return trace(newReflectionRay, depth + 1) + lighting.light(intersectionData);
      }
      else if (intersectionData.mat.type == MaterialType::REFRACTIVE) {
        Vec3 refractionColor;
        float kr;
        Vec3 dir = ray.dir;
        Vec3 N = intersectionData.pNNonSmooth;
        fresnel(dir, N, intersectionData.mat.ior, kr);
        bool outside = glm::dot(dir, N) < 0;
        Vec3 bias = reflectionBias * N;
        // compute refraction if it is not a case of total internal reflection
        if (kr < 1) {
          Ray refrRay(
            outside ? intersectionData.p - bias : intersectionData.p + bias,
            refract(dir, N, intersectionData.mat.ior)
          );
          refractionColor = trace(refrRay, depth + 1);
        }

        Ray newReflectionRay(
          outside ? intersectionData.p + bias : intersectionData.p - bias,
          glm::reflect(dir, N)
        );
        Vec3 reflectionColor = trace(newReflectionRay, depth + 1);

        // mix the two
        return reflectionColor * kr + refractionColor * (1 - kr);
      }
    }

    return scene.settings.backgroundColor;
  }
private:
  Scene& scene;
  Lighting& lighting;
  AABBTree& aabb;
  float reflectionBias = 0.001f;

  /// Taken from the Scratch a pixel tutorial on refraction
  Vec3 refract(const Vec3& I, const Vec3& N, const float& ior)
  {
    float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(I, N));
    float etai = 1, etat = ior;
    Vec3 n = N;
    if (cosi < 0) { cosi = -cosi; }
    else { std::swap(etai, etat); n = -N; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vec3() : eta * I + (eta * cosi - sqrtf(k)) * n;
  }

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
