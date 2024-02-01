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
    /* Sample the pixel at its center */
    x += 0.5f;
    y += 0.5f;

    /* Transform to NDC - 0.0 - 1.0 */
    x /= width;
    y /= height;

    /* To Screen space - bottom left is [-1;-1] and top right is [1;1] */
    x = (x * 2) - 1;
    y = 1 - (y * 2);

    /* Consider the aspect ratio */
    x *= aspRat;

    Vec3 rayDir(x, y, focalDist);

    return Ray(camera.pos, glm::normalize(camera.applyTransformation(rayDir)));
  }

private:
  int32_t width;
  int32_t height;
  float aspRat;
  const Camera& camera;
  float focalDist;
};

#endif // !RAYGEN_H
