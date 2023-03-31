#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>

#include "src/out/ppm/PPMImageMeta.h"
#include "src/out/ppm/PPMImage.h"
#include "src/out/ppm/PPMImageWriter.h"
#include "src/out/ppm/PPMImageFileWriter.h"

#include "src/sampling/Raygen.h"

#include "src/utils/TypeDefs.h"

const int16_t RENDER_WIDTH = 640;
const int16_t RENDER_HEIGHT = 480;
const int16_t MAX_COLOR = 255;

int main() {
  std::vector<Triangle> triangles;
  Triangle a(Vec3(-1.75, -1.75, -3), Vec3(1.75, -1.75, -3), Vec3(0, 1.75, -3));

  PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
  PPMImage image(imageMetadata);
  Vec3 cameraPos{};
  Raygen s(RENDER_WIDTH, RENDER_HEIGHT, cameraPos, -1);

  for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
    for (int32_t col = 0; col < RENDER_WIDTH; col++) {
      Ray& ray = s.gen(col, row);

      if (a.intersect(ray)) {
        image.writePixel(Color(255, 0, 0));
      }
      else {
        image.writePixel(Color(0, 255, 0));
      }
    }
  }

  PPMImageFileWriter fileWriter(image, "out.ppm");
  fileWriter.write();

    return 0;
}
