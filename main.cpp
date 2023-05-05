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
#include "src/sampling/Camera.h"

#include "src/utils/TypeDefs.h"

const int16_t RENDER_WIDTH = 640;
const int16_t RENDER_HEIGHT = 480;
const int16_t MAX_COLOR = 255;

int main() {
  std::vector<Triangle> triangles = {
    Triangle(Vec3(-0.5, 1, -3.5), Vec3(0.5, 1, -3.5), Vec3(0, 2, -3.5), Color{0,0,255}),
    Triangle(Vec3(0, 1, -3.2), Vec3(2, 1, -3.2), Vec3(1, 2, -3.2), Color{255, 0, 0 })
  };
  PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
  PPMImage image(imageMetadata);

  Camera camera(Vec3{0.0f, 0.0f, 1.0f});
  camera.dolly(0.3);
  camera.truck(0.3);
  camera.pedestal(0.3);
  camera.pan(10);
  camera.tilt(10);
  camera.roll(10);
  Raygen s(RENDER_WIDTH, RENDER_HEIGHT, camera, -1);

  for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
    for (int32_t col = 0; col < RENDER_WIDTH; col++) {
      Ray& ray = s.gen(col, row);
      bool intersects = false;
      Color currentColor;

      for (Triangle& tr : triangles) {
        if (tr.intersect(ray)) {
          currentColor = tr.col;
          intersects = true;
        }
      }
      
      if (!intersects) {
        image.writePixel(Color{ 0, 255, 0 });
      }
      else {
        image.writePixel(currentColor);
      }
    }
  }

  PPMImageFileWriter fileWriter(image, "out.ppm");
  fileWriter.write();

  return 0;
}
