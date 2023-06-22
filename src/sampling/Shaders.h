#ifndef SHADERS_H

#include <utility>

#include <utils/TypeDefs.h>

int maxDepth = 5;
float refractionBias = 0.01f;

inline InternalColor shadeReflect(Ray reflectionRay, Material material, IntersectionData intrsData, int depth, std::vector<Object>& objects,
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
        else {
          return shadeReflect(reflectionRay, obj.mat, intersectionData, depth + 1, objects, lights, backgroundColor);
        }
      }
    }
  }

  return backgroundColor;
}

#endif // !SHADERS_H
