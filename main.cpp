#include <iostream>
#include <cstdint>
#include <fstream>

#include "src/out/ppm/PPMImageMeta.h"
#include "src/out/ppm/PPMImage.h"
#include "src/out/ppm/PPMImageWriter.h"
#include "src/out/ppm/PPMImageFileWriter.h"

#include "src/sampling/SimpleSampler.h"

#include "src/utils/TypeDefs.h"

const int16_t RENDER_WIDTH = 640;
const int16_t RENDER_HEIGHT = 480;
const int16_t MAX_COLOR = 255;

Color vis(glm::vec3 rayDir) {
  rayDir = glm::abs(rayDir) * static_cast<float>(MAX_COLOR);
  return Color((int32_t)rayDir.x, (int32_t)rayDir.y, (int32_t)rayDir.z);
}

int main() {
    PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
    PPMImage image(imageMetadata);
    glm::vec3 cameraPos;
    Raygen s(RENDER_WIDTH, RENDER_HEIGHT, cameraPos, -1);

    for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
      for (int32_t col = 0; col < RENDER_WIDTH; col++) {
        image.writePixel(vis(s.gen(col, row).dir));
      }
    }

    PPMImageFileWriter fileWriter(image, "out.ppm");
    fileWriter.write();

    return 0;
}
