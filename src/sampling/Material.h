#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <utils/TypeDefs.h>

struct Material {
  std::string type;
  Vec3 albedo{};
  bool smoothShading = false;
  float ior{};
};

#endif // !MATERIAL_H
