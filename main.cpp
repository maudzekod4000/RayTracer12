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
#include "src/scene/Scene.h"

#include "src/utils/TypeDefs.h"

const int16_t MAX_COLOR = 255;

int main() {
  std::cout << "Parsing scene object..." << '\n';
  Scene scene("scene1.crtscene");
  std::cout << "Completed parsing scene object" << '\n';

  uint32_t RENDER_WIDTH = scene.settings.imageSettings.width;
  uint32_t RENDER_HEIGHT = scene.settings.imageSettings.heigth;
  PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
  PPMImage image(imageMetadata);

  Camera camera(scene.camera.position, scene.camera.matrix);
  Raygen s(RENDER_WIDTH, RENDER_HEIGHT, camera, -1);

  std::cout << "Rendering..." << '\n';

  for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
    for (int32_t col = 0; col < RENDER_WIDTH; col++) {
      Ray& ray = s.gen(col, row);
      bool intersects = false;
      Color currentColor;

      for (Triangle& tr : scene.triangles) {
        if (tr.intersect(ray)) {
          currentColor = tr.col;
          intersects = true;
        }
      }
      
      if (!intersects) {
        image.writePixel(Color(scene.settings.backgroundColor));
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
