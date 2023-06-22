#ifndef RAY_H
#define RAY_H

#include <utils/TypeDefs.h>

struct Ray {
  Vec3 origin{};
  Vec3 dir{};
  uint32_t depth{};
};

#endif // !RAY_H
