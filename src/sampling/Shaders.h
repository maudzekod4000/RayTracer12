#ifndef SHADERS_H

#include <utils/TypeDefs.h>

InternalColor shadeDiffuse(const Material mat) {
  return mat.albedo;
}

#endif // !SHADERS_H
