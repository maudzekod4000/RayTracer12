#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "Triangle.h"
#include "Material.h"

struct Object {
  std::vector<Triangle> triangles;
  Material mat;
};

#endif // !OBJECT_H
