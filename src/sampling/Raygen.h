#ifndef RAYGEN_H
#define RAYGEN_H

#include <utils/TypeDefs.h>
#include "sampling/Camera.h"
#include "sampling/Ray.h"

struct Raygen {
  inline Raygen(int32_t w, int32_t h, const Camera& camera, float focalDist):
    width(w), height(h), aspRat(static_cast<float>(w) / h), camera(camera), focalDist(focalDist) {

  }
  /// <summary>
  /// Creates a ray from camera position to an image plane.
  /// x, y are in raster coordinates.
  /// </summary>
  inline Ray gen(float x, float y) {
    
  }

private:
  int32_t width;
  int32_t height;
  float aspRat;
  const Camera& camera;
  float focalDist;
};

#endif // !RAYGEN_H
