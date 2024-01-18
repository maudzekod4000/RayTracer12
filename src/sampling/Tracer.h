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

  inline InternalColor trace(const Ray& ray, int depth) {
    if (depth > 20) {
      return Vec3();
    }
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
      else if (intersectionData.mat.type == MaterialType::REFRACTIVE) {
        Vec3 refractionColor;
        // compute fresnel
        float kr;
        Vec3 N = intersectionData.pNNonSmooth;
        Vec3 rayDir = ray.dir;
        fresnel(rayDir, N, 1.5f, kr);
        bool outside = glm::dot(rayDir, N) < 0;
        Vec3 bias = 0.001f * N;
        // compute refraction if it is not a case of total internal reflection
        if (kr < 1) {
          Vec3 refractionDirection = glm::normalize(refract(rayDir, N, 1.5f));
          Vec3 refractionRayOrig = outside ? intersectionData.p - bias : intersectionData.p + bias;
          Ray newReflectionRay{
           refractionRayOrig,
           refractionDirection
          };
          refractionColor = trace(newReflectionRay, depth + 1);
        }

        Vec3 reflOrig = intersectionData.p + (N * reflectionBias);
        Vec3 reflectionRayOrig = outside ? reflOrig + bias : reflOrig - bias;
        Ray newReflectionRay{
          reflectionRayOrig,
          glm::normalize(glm::reflect(rayDir, N))
        };
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
  float reflectionBias = 0.01f;

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
