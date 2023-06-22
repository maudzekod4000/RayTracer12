#ifndef PPM_COLOR_H
#define PPM_COLOR_H

#include <utils/TypeDefs.h>

struct PPMColor {
  inline PPMColor(uint8_t r, uint8_t g, uint8_t b) :r(r), g(g), b(b) {}
  inline PPMColor() : r(0), g(0), b(0) {}
  uint8_t r;
  uint8_t g;
  uint8_t b;

  inline static PPMColor from(const InternalColor& color) {
    float r = glm::clamp(color.r, 0.0f, 1.0f);
    float g = glm::clamp(color.g, 0.0f, 1.0f);
    float b = glm::clamp(color.b, 0.0f, 1.0f);
    return PPMColor(static_cast<uint8_t>(r * MAX_COLOR), static_cast<uint8_t>(g * MAX_COLOR), static_cast<uint8_t>(b * MAX_COLOR));
  }
};

#endif // !PPM_COLOR_H
