#ifndef CAMERA_H
#define CAMERA_H

#include <utils/TypeDefs.h>

struct Camera {
  inline Camera(const Vec3& pos, const Vec3 rotationAxis, float rotationAngle) :
    pos(pos) {
    rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);
  }

  inline Vec3 pan(const Vec3& rayDir) const {
    return glm::vec4(rayDir, 1.0f) * rotationMatrix;
  }

  Vec3 pos;
  glm::mat4 rotationMatrix;
};

#endif // !CAMERA_H
