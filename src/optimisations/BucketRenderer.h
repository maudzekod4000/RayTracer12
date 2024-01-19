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

  // Creates buckets and starts the thread pool on creation
  BucketRenderer(Raygen& raygen, Tracer& tracer, Scene& scene, PPMImage& image) :
    rayGen(raygen),
    tracer(tracer),
    scene(scene),
    imageBuffer(image) {

    buildBuckets(); threadPool.start();
  }

  // Iterates the buckets and gives them to the thread pool for execution.
  void render() {
    while (!buckets.empty()) {
      Bucket bucket = buckets.top();
      buckets.pop();

      auto renderJob = [bucket, this]() {
        for (int32_t row = bucket.y; row < bucket.y + bucketSize; row++) {
          for (int32_t col = bucket.x; col < bucket.x + bucketSize; col++) {
            Ray ray = rayGen.gen(col, row);
            imageBuffer.writePixel(row, col, PPMColor::from(tracer.trace(ray, 0)));
          }
        }
      };
      threadPool.doJob(renderJob);
    }
  }
private:
  ThreadPool threadPool;
  Raygen& rayGen;
  std::stack<Bucket> buckets;
  Tracer& tracer;
  Scene& scene;
  PPMImage& imageBuffer;
  int32_t bucketSize = 20; // TODO: When this size is changed, the rendering crashes.

  // Splits the image plane into squares for parallel rendering.
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
