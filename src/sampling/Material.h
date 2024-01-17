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
  MaterialType type;
  Vec3 albedo{};
  bool smoothShading = false;
  float ior{};
};

#endif // !MATERIAL_H
