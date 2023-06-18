#ifndef SHADERS_H

#include <utils/TypeDefs.h>

InternalColor shadeDiffuse(const Material mat) {
  return mat.albedo;
}

int maxDepth = 5;

// This will be called recursively, so what arguments does it need?
// The reflection ray, the material (at least the albedo), lights, objects, ray depth? or maybe it will be a field on the ray.
// Intersection data
InternalColor shadeReflect(Ray reflectionRay, Material material, IntersectionData intrsData, int depth, std::vector<Object>& objects,
  Lighting& lights, Vec3 backgroundColor) {
  
  if (material.type == "diffuse") {
    return material.albedo + lights.light(intrsData);
  }
  else if (depth > maxDepth) {
    return material.albedo;
  }

  IntersectionData intersectionData{};

  for (Object& obj : objects) {
    for (Triangle& tr : obj.triangles) {
      if (tr.intersect(reflectionRay, intersectionData)) {
        if (obj.mat.type == "reflective") {
          Ray newReflectionRay{intrsData.p, glm::reflect(reflectionRay.dir, intersectionData.pN ) +
            (intersectionData.pN * 0.01f), 0 };
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
