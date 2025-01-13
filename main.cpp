#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>
#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>

#include "src/out/ppm/PPMImageMeta.h"
#include "src/out/ppm/PPMImage.h"
#include "src/out/ppm/PPMImageWriter.h"
#include "src/out/ppm/PPMImageFileWriter.h"

#include "src/sampling/Raygen.h"
#include "src/sampling/Camera.h"
#include "src/scene/Scene.h"
#include "out/ppm/PPMColor.h"
#include "src/utils/TypeDefs.h"
#include "sampling/Tracer.h"

int main(int argc, char* argv[]) {
  std::cout << "Parsing scene object..." << '\n';
  Scene scene(argv[1]);
  std::cout << "Completed parsing scene object" << '\n';

  int32_t RENDER_WIDTH = scene.settings.imageSettings.width;
  int32_t RENDER_HEIGHT = scene.settings.imageSettings.heigth;
  PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
  PPMImage image(imageMetadata);

  Camera camera(scene.camera.position, scene.camera.matrix);
  Raygen rayGenerator(RENDER_WIDTH, RENDER_HEIGHT, camera, -1);
  PPMColor backGroundColor = PPMColor::from(scene.settings.backgroundColor);
  LightOptions lightOptions{ 0.01f, 0.5f };
  Lighting lighting(lightOptions, scene.lights, scene.objects);
  Tracer tracer(scene, lighting);

  std::cout << "Rendering..." << '\n';
  auto start = std::chrono::steady_clock::now();

  for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
    for (int32_t col = 0; col < RENDER_WIDTH; col++) {
      Ray& ray = rayGenerator.gen(col, row);
      image.writePixel(PPMColor::from(tracer.trace(ray, 0)));
    }
  }

  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsedSeconds = end - start;
  std::cout << "Rendering took " << elapsedSeconds.count() << " sec." << '\n';

  PPMImageFileWriter fileWriter(image, "out.ppm");
  fileWriter.write();

  return 0;
}
