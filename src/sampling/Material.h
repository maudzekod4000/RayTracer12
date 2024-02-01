#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <utils/TypeDefs.h>

enum class MaterialType {
  DIFFUSE,
  REFLECTIVE,
  REFRACTIVE,
  NONE
};

struct Material {
  Material() : type(MaterialType::NONE), albedo(0.0f), smoothShading(false), ior(1.0f) {}
  MaterialType type;
  Vec3 albedo;
  bool smoothShading;
  float ior;
};

#endif // !MATERIAL_H
