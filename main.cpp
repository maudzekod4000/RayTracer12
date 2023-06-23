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
#include "optimisations/ThreadPool.h"
#include "optimisations/BucketRenderer.h"

int main() {
  std::cout << "Parsing scene object..." << '\n';
  Scene scene("reflscene5.crtscene");
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

  auto start = std::chrono::steady_clock::now();

  {
    BucketRenderer bucketRenderer(rayGenerator, tracer, scene, image);

    std::cout << "Rendering..." << '\n';

    bucketRenderer.render();
  }

  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsedSeconds = end - start;
  std::cout << "Rendering took " << elapsedSeconds.count() << " sec." << '\n';

  PPMImageFileWriter fileWriter(image, "out.ppm");
  fileWriter.write();

  return 0;
}
