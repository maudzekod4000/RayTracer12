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
      return scene.settings.backgroundColor;
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
        return trace(newReflectionRay, depth + 1);
      }
      else if (intersectionData.mat.type == MaterialType::REFRACTIVE) {
        Vec3 I = ray.dir;
        Vec3 N = intersectionData.pN;
        bool leavesTransparentObject = glm::dot(I, N) > 0;
        float n2 = intersectionData.mat.ior;
        float n1 = 1.0f;
        if (leavesTransparentObject) {
          N = -N;
          std::swap(n2, n1);
        }
        float cosA = -glm::dot(I, N);
        float snell = (glm::sqrt(1.0f - (cosA * cosA) * n1)) / n2;
        Vec3 C = glm::normalize(I + cosA * N);
        Vec3 B = C * snell;
        Vec3 A = glm::sqrt(1 - (snell * snell)) * -N;
        Vec3 R = A + B;
        Vec3 bias = N * reflectionBias;
        Ray refrRay{
          intersectionData.p + bias,
          R
        };
        return trace(refrRay, depth + 1);
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
  /*Vec3 refract(const Vec3& I, const Vec3& N, const float& ior)
  {
    float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(I, N));
    float etai = 1, etat = ior;
    Vec3 n = N;
    if (cosi < 0) { cosi = -cosi; }
    else { std::swap(etai, etat); n = -N; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vec3() : eta * I + (eta * cosi - sqrtf(k)) * n;
  }*/

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

  inline Vec3 refract(const Vec3& uv, const Vec3& n, float etai_over_etat) {
    float cos_theta = fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -sqrt(std::fabs(1.0f - glm::length(r_out_perp) * glm::length(r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
  }
};

#endif // !TRACER_H
