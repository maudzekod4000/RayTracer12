#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#include "src/out/ppm/PPMImageMeta.h"
#include "src/out/ppm/PPMImage.h"
#include "src/out/ppm/PPMImageWriter.h"
#include "src/out/ppm/PPMImageFileWriter.h"

#include "src/sampling/Raygen.h"

#include "src/utils/TypeDefs.h"

const int16_t RENDER_WIDTH = 640;
const int16_t RENDER_HEIGHT = 480;
const int16_t MAX_COLOR = 255;

std::vector<Triangle> circle(Vec3 center, float r, float angleDeg) {
  std::vector<Triangle> tris;

  Vec3 firstPoint = Vec3(center.x + 1, center.y, center.z);

  for (float degrees = angleDeg; degrees <= 360.0f; degrees += angleDeg) {
    Vec3 secondPoint = Vec3(glm::cos(glm::radians(degrees)), glm::sin(glm::radians(degrees)), center.z);
    tris.emplace_back(center, firstPoint, secondPoint);
    firstPoint = secondPoint;
  }

  return tris;
}

int main() {
  std::vector<Triangle> triangles = circle(Vec3(0, 0, -3), 1, 10.0f);


  PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
  PPMImage image(imageMetadata);
  Vec3 cameraPos{};
  Raygen s(RENDER_WIDTH, RENDER_HEIGHT, cameraPos, -1);

  for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
    for (int32_t col = 0; col < RENDER_WIDTH; col++) {
      Ray& ray = s.gen(col, row);
      bool intersects = false;

      for (Triangle& tr : triangles) {
        if (tr.intersect(ray)) {
          image.writePixel(tr.col);
          intersects = true;
          break;
        }
      }
      
      if (!intersects) {
        image.writePixel(Color(0, 255, 0));
      }
    }
  }

  PPMImageFileWriter fileWriter(image, "out.ppm");
  fileWriter.write();

    return 0;
}
