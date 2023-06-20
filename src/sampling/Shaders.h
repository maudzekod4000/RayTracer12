#ifndef SHADERS_H

#include <utils/TypeDefs.h>

#include <utility>

InternalColor shadeDiffuse(const Material mat) {
  return mat.albedo;
}

int maxDepth = 5;
float refractionBias = 0.01f;

InternalColor shadeReflect(Ray reflectionRay, Material material, IntersectionData intrsData, int depth, std::vector<Object>& objects,
  Lighting& lights, Vec3 backgroundColor) {
  
  if (material.type == "diffuse") {
    return material.albedo + lights.light(intrsData);
  }
  else if (material.type == "constant") {
    return material.albedo;
  }
  else if (depth > maxDepth) {
    return material.albedo;
  }

  IntersectionData intersectionData{};

  for (Object& obj : objects) {
    for (Triangle& tr : obj.triangles) {
      if (tr.intersect(reflectionRay, intersectionData)) {
        if (obj.mat.type == "reflective") {
          Ray newReflectionRay{intrsData.p + (intersectionData.pN * 0.01f), glm::reflect(reflectionRay.dir, intersectionData.pN ), 0 };
          return shadeReflect(newReflectionRay, obj.mat, intersectionData, depth + 1, objects, lights, backgroundColor);
        }
        else if (obj.mat.type == "refractive") {
          float n1 = 1.0f;
          float n2 = obj.mat.ior;
          Vec3 N = intersectionData.pN;
          // Check if we are entering or exiting an object
          // If the ray direction and the normal point to the same direction, we are exiting an object.
          if (glm::dot(reflectionRay.dir, N) > 0) {
            N = -N;
            std::swap(n1, n2);
          }

          Vec3 refractionDir = glm::refract(reflectionRay.dir, N, n1 / n2);

          if (glm::length(refractionDir) != 0.0f) {
            Ray refractionRay{ intersectionData.p + (-N * refractionBias), refractionDir, 0 };
            InternalColor refractionColor = shadeReflect(
              refractionRay, obj.mat, intersectionData, depth + 1, objects, lights, backgroundColor
            );

            Ray newReflectionRay{ intrsData.p + (intersectionData.pN * 0.01f), glm::reflect(reflectionRay.dir, intersectionData.pN), 0 };
            InternalColor reflectionColor = shadeReflect(newReflectionRay, obj.mat, intersectionData, depth + 1, objects, lights, backgroundColor);

            float fresnel = 0.5f * glm::pow((1.0f + glm::dot(reflectionRay.dir, N)), 5);

            return fresnel * reflectionColor + (1 - fresnel) * refractionColor;
          }
          else {
            Ray newReflectionRay{ intrsData.p + (intersectionData.pN * 0.01f), glm::reflect(reflectionRay.dir, intersectionData.pN), 0 };
            return shadeReflect(newReflectionRay, obj.mat, intersectionData, depth + 1, objects, lights, backgroundColor);
          }
        }
        else {
          return shadeReflect(reflectionRay, obj.mat, intersectionData, depth + 1, objects, lights, backgroundColor);
        }
      }
    }
  }

  return backgroundColor;
}

#endif // !SHADERS_H
