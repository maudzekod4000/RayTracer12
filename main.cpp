#include <iostream>
#include <cstdint>
#include <fstream>

#include "src/out/ppm/PPMImageMeta.h"
#include "src/out/ppm/PPMImage.h"
#include "src/out/ppm/PPMImageWriter.h"
#include "src/out/ppm/PPMImageFileWriter.h"

#include "src/utils/TypeDefs.h"

#include "src/letters/LetterSegment.h"
#include "src/letters/Letter.h"
#include "src/letters/LetterFactory.h"

const int16_t RENDER_WIDTH = 640;
const int16_t RENDER_HEIGHT = 480;
const int16_t MAX_COLOR = 255;

struct Circle {
  Point2D pos;
  float radius;
};

Circle someCircle{ Point2D(RENDER_WIDTH / 2, RENDER_HEIGHT / 2), 150.0f };

bool isPixelInCircle(Point2D pixelPos, const Circle& circle) {
  float dist = glm::length(circle.pos - pixelPos);
  return dist <= circle.radius;
}

Color sampleProceduralTexture(Point2D samplingPoint, const int32_t rectsPerRow, int32_t rectsPerCol) {
  int32_t rectWidth = RENDER_WIDTH / rectsPerRow;
  int32_t rectHeight = RENDER_HEIGHT / rectsPerCol;

  int32_t colMask = samplingPoint.x / rectWidth;
  int32_t rowMask = samplingPoint.y / rectHeight;

  // Let's combine col and row mask for simplicity and we want to choose from 3 color values - R, G, B
  int32_t mask = (colMask + rowMask) % 3;
  const int32_t maxTextureColor = MAX_COLOR / 2;

  switch (mask) {
  case 0:
    return Color(rand() % MAX_COLOR, rand() % maxTextureColor, rand() % maxTextureColor);
  case 1:
    return Color(rand() % maxTextureColor, rand() % MAX_COLOR, rand() % maxTextureColor);
  case 2:
    return Color(rand() % maxTextureColor, rand() % maxTextureColor, rand() % MAX_COLOR);
  }

  return Color(maxTextureColor);
}

bool doesIntersectLetter(Point2D point) {  
  Point2D letterPos{ 100, 100 };
  int32_t x = point.x;
  int32_t y = point.y;

  if (x < letterPos.x || y < letterPos.y) {
    return false;
  }

  float slope = -5.0f;
  float yIntercept = letterPos.y + 10;

  float letterHeight = 40;

  float expectedY = slope * (x - letterPos.x) + yIntercept;
  float delta = 20;

  if (fabs(expectedY - y) <= delta) {
    return true;
  }


  return false;
}

Letter v = LetterFactory().letters['V'];
Letter r = LetterFactory().letters['R'];
Letter a = LetterFactory().letters['A'];
Letter y = LetterFactory().letters['Y'];


// TODO: Implenent the alphabet and write something - "Zdrasti lektore"
int main() {
    using namespace std;
    PPMImageMeta imageMetadata(RENDER_WIDTH, RENDER_HEIGHT, MAX_COLOR);
    PPMImage image(imageMetadata);

   
    v.setPosition(Point2D(100, 100));
    r.setPosition(Point2D(150, 100));
    a.setPosition(Point2D(200, 100));
    y.setPosition(Point2D(250, 100));
    

    for (int32_t row = 0; row < RENDER_HEIGHT; row++) {
      for (int32_t col = 0; col < RENDER_WIDTH; col++) {

        /*if (isPixelInCircle({col, row}, someCircle)) {
          image.writePixel(Color(58, 118, 35));
        }
        else {
          image.writePixel(Color(183, 183, 183));
        }*/

        //image.writePixel(sampleProceduralTexture({ col, row }, 4, 4));
        Point2D point{ col, row };
        
        if (a.intersect(point) ||
            v.intersect(point) ||
            r.intersect(point) ||
          y.intersect(point)) {
          image.writePixel(Color(255, 255, 255));
        }
        else {
          image.writePixel(Color());
        }
      }
    }

    PPMImageFileWriter fileWriter(image, "greenhole.ppm");
    fileWriter.write();

    return 0;
}
