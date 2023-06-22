#ifndef BUCKET_RENDERER_H
#define BUCKET_RENDERER_H

#include <stack>
#include <mutex>
#include <thread>

#include "Bucket.h"
#include "sampling/Raygen.h"
#include "sampling/Tracer.h"
#include "out/ppm/PPMColor.h"
#include "out/ppm/PPMImage.h"
#include "scene/Scene.h"

struct BucketRenderer {
  BucketRenderer(Raygen& raygen, Tracer& tracer, Scene& scene, PPMImage& image) :
    rayGen(raygen),
    tracer(tracer),
    scene(scene),
    imageBuffer(image) {
    buildBuckets(); threadPool.start();
  }

  void render() {
    while (true) {
      std::lock_guard<std::mutex> bucketGuard(bucketMutex);

    }
  }
private:
  ThreadPool threadPool;
  Raygen& rayGen;
  std::stack<Bucket> buckets;
  Tracer& tracer;
  Scene& scene;
  PPMImage& imageBuffer;
  std::mutex bucketMutex;
  int32_t bucketSize = 20;

  void buildBuckets() {
    int32_t width = scene.settings.imageSettings.width;
    int32_t height = scene.settings.imageSettings.heigth;

    for (int32_t h = 0; h < height; h += bucketSize) {
      for (int32_t w = 0; w < width; w += bucketSize) {
        buckets.push(Bucket{ w, h });
      }
    }
  }
};

#endif // !BUCKET_RENDERER_H
