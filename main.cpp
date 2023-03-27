#include <iostream>
#include <cstdint>
#include <fstream>

#include "src/out/ppm/PPMImageMeta.h"
#include "src/out/ppm/PPMImage.h"
#include "src/out/ppm/PPMImageWriter.h"
#include "src/out/ppm/PPMImageFileWriter.h"

#include "src/sampling/Raygen.h"

#include "src/utils/TypeDefs.h"

const int16_t RENDER_WIDTH = 640;
const int16_t RENDER_HEIGHT = 480;
const int16_t MAX_COLOR = 255;

Color vis(Vec3 rayDir) {
  rayDir = glm::abs(rayDir) * static_cast<float>(MAX_COLOR);
  return Color((int32_t)rayDir.x, (int32_t)rayDir.y, (int32_t)rayDir.z);
}

int main() {
  Triangle a(Vec3(-1.75, -1.75, -3), Vec3(1.75, -1.75, -3), Vec3(0, 1.75, -3));
  std::cout << a.normal.x << ";" << a.normal.y << ";" << a.normal.z << std::endl;

  std::cout << "The area is: " << a.area() << std::endl;
    /*PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
    PPMImage image(imageMetadata);
    Vec3 cameraPos;
    Raygen s(RENDER_WIDTH, RENDER_HEIGHT, cameraPos, -1);

    for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
      for (int32_t col = 0; col < RENDER_WIDTH; col++) {
        image.writePixel(vis(s.gen(col, row).dir));
      }
    }

    PPMImageFileWriter fileWriter(image, "out.ppm");
    fileWriter.write();*/

    return 0;
}
