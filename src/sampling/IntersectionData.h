#ifndef INTERSECTION_DATA_H
#define INTERSECTION_DATA_H

#include "utils/TypeDefs.h"
#include "sampling/Material.h"

struct IntersectionData {
  Vec3 p{};
  Vec3 pN{};
  Vec3 hitBaryCentricCoordinates{};
  Vec3 pNNonSmooth{};
  Material mat{};
  float t = 9999.9f;
  bool intersection = false;
};

#endif // !INTERSECTION_DATA_H
