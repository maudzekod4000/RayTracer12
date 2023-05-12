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

int main() {
  std::cout << "Parsing scene object..." << '\n';
  Scene scene("scene0.crtscene");
  std::cout << "Completed parsing scene object" << '\n';

  uint32_t RENDER_WIDTH = scene.settings.imageSettings.width;
  uint32_t RENDER_HEIGHT = scene.settings.imageSettings.heigth;
  PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
  PPMImage image(imageMetadata);

  Camera camera(scene.camera.position, scene.camera.matrix);
  Raygen rayGenerator(RENDER_WIDTH, RENDER_HEIGHT, camera, -1);
  PPMColor backGroundColor = PPMColor::from(scene.settings.backgroundColor);
  LightOptions lightOptions{ 0.001f, 0.3f };
  Lighting lighting(lightOptions, scene.lights, scene.triangles);

  std::cout << "Rendering..." << '\n';

  for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
    for (int32_t col = 0; col < RENDER_WIDTH; col++) {
      Ray& ray = rayGenerator.gen(col, row);
      InternalColor currentColor{};
      IntersectionData intersectionData{};

      for (Triangle& tr : scene.triangles) {
        if (tr.intersect(ray, intersectionData)) {
          currentColor = tr.col;
        }
      }

      if (!intersectionData.intersection) {
        image.writePixel(backGroundColor);
      }
      else {
        currentColor += lighting.light(intersectionData);
        image.writePixel(PPMColor::from(currentColor));
      }
    }
  }

  PPMImageFileWriter fileWriter(image, "out.ppm");
  fileWriter.write();

  return 0;
}
