#include <iostream>
#include <cstdint>
#include <fstream>

#include "src/out/ppm/PPMImageMeta.h"
#include "src/out/ppm/PPMImage.h"
#include "src/out/ppm/PPMImageWriter.h"
#include "src/out/ppm/PPMImageFileWriter.h"

#include "src/utils/TypeDefs.h"

const int16_t RENDER_WIDTH = 640;
const int16_t RENDER_HEIGHT = 480;
const int16_t MAX_COLOR = 255;

int main() {
    using namespace std;
    PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
    PPMImage image(imageMetadata);

    float radius = 150.0; // px
    Point2D circlePos = Point2D(RENDER_WIDTH / 2, RENDER_HEIGHT / 2);

    for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
      for (int32_t col = 0; col < RENDER_WIDTH; col++) {
        float dist = glm::length(circlePos - Point2D(col, row));

        if (dist <= radius) {
          image.writePixel(Color(58, 118, 35));
        }
        else {
          image.writePixel(Color(183, 183, 183));
        }
        
      }
    }

    PPMImageFileWriter fileWriter(image, "greenhole.ppm");
    fileWriter.write();

    return 0;
}
