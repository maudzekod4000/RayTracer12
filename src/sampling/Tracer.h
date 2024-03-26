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
    if (depth > 8) {
      return scene.settings.backgroundColor;
    }
    IntersectionData intersectionData = aabb.intersectAABBTree(ray);

    if (intersectionData.intersection) {
      if (intersectionData.mat.type == MaterialType::DIFFUSE || intersectionData.mat.type == MaterialType::NONE) {
        return intersectionData.mat.albedo + shadeDiffuse(ray, intersectionData, depth + 1);
      }
      else if (intersectionData.mat.type == MaterialType::REFLECTIVE) {
        Vec3 N = intersectionData.mat.smoothShading ? intersectionData.pN : intersectionData.pNNonSmooth;
        bool outside = glm::dot(ray.dir, N) < 0;
        Vec3 bias = refractionBias * N;
        Ray newReflectionRay(
          outside ? intersectionData.p + bias : intersectionData.p - bias,
          glm::reflect(ray.dir, N)
        );
        return intersectionData.mat.albedo + trace(newReflectionRay, depth + 1);
      }
      else if (intersectionData.mat.type == MaterialType::REFRACTIVE) {
        Vec3 I = ray.dir;
        Vec3 N = intersectionData.mat.smoothShading ? intersectionData.pN : intersectionData.pNNonSmooth;
        float n1 = 1.0f;
        float n2 = intersectionData.mat.ior;
        float angleIN = glm::dot(I, N);

        if (angleIN > 0) {
          N = -N;
          std::swap(n1, n2);
        }

        float cosA = -angleIN;
        float sinA = glm::sqrt(glm::max(0.0f, 1.0f - angleIN * angleIN));
        float sinB = (sinA * n1) / n2;
        float cosB = glm::sqrt(glm::max(0.0f, 1.0f - sinB * sinB));

        NormalizedColor refrCol(0.0f);
        float fresnel = 1;
        if (sinA < n1 / n2) {
          Vec3 C = glm::normalize(I + cosA * N);
          Vec3 B = C * sinB;
          Vec3 A = cosB * -N;
          Vec3 R = A + B;

          Ray refr{
            intersectionData.p + (-N * refractionBias),
            R
          };
          refrCol = trace(refr, depth + 1);
          float absCosA = fabsf(cosA);
          float Rs = ((n2 * absCosA) - (n1 * cosB)) / ((n2 * absCosA) + (n1 * cosB));
          float Rp = ((n1 * absCosA) - (n2 * cosB)) / ((n1 * absCosA) + (n2 * cosB));
          fresnel = (Rs * Rs + Rp * Rp) / 2.0f;
        }

        Ray newReflectionRay(
          intersectionData.p + (N * reflectionBias),
          glm::reflect(I, N)
        );
        NormalizedColor reflCol = trace(newReflectionRay, depth + 1);
        return fresnel * reflCol + (1.0f - fresnel) * refrCol;
      }
    }

    return scene.settings.backgroundColor;
  }
private:
  Scene& scene;
  Lighting& lighting;
  AABBTree& aabb;
  float refractionBias = 0.0001;
  float reflectionBias = 0.0004;
  const size_t diffuseReflCount = 8;

  inline NormalizedColor shadeDiffuse(const Ray& ray, const IntersectionData& intrs, int depth) {
      NormalizedColor diffReflColor(0.0f);

      for (size_t i = 0; i < diffuseReflCount; i++) {
          Vec3 rightAxis = glm::normalize(glm::cross(ray.dir, intrs.pN));
          Vec3 upAxis = intrs.pN;
          Vec3 forwardAxis = glm::cross(rightAxis, upAxis);

          glm::mat3x3 localHitMatrix(1.0f);
          localHitMatrix[0] = rightAxis;
          localHitMatrix[1] = forwardAxis;
          localHitMatrix[2] = upAxis;

          // Generate random angle in the XY plane
          float randAngleInXY = M_PI * (rand() / RAND_MAX);
          // COnstruct random vector in the XY plane
          Vec3 randVecXY = Vec3(cos(randAngleInXY), sin(randAngleInXY), 0);

          // Generate random angle in the XZ plane
          float randAngleINXZ = M_PI * 2 * (rand() / RAND_MAX);
          glm::mat3x3 rotMatY(1.0f);
          rotMatY[0][0] = cos(randAngleINXZ);
          rotMatY[2][0] = -sin(randAngleINXZ);
          rotMatY[0][2] = sin(randAngleINXZ);
          rotMatY[2][2] = cos(randAngleINXZ);

          Vec3 randVecInXYRotated = randVecXY * rotMatY;

          Vec3 diffReflRayDir = randVecInXYRotated * localHitMatrix;
          Vec3 diffRayOrigin = intrs.p + (intrs.pN * reflectionBias);
          Ray diffRay(diffRayOrigin, diffReflRayDir);

          diffReflColor = diffReflColor + trace(diffRay, depth + 1);
      }

      NormalizedColor clr = lighting.light(intrs);

      diffReflColor = diffReflColor + clr;

      return diffReflColor / (float)(diffuseReflCount + 1);
  }
};

#endif // !TRACER_H
