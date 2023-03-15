#ifndef LETTER_SEGMENT_H
#define LETTER_SEGMENT_H

#include "utils/TypeDefs.h"

class LetterSegment {
public:
  LetterSegment(Point2D segmentPos, float segmentSlope, float letterHeight /*y-intercept*/, float fontSize, float maxWidth) :
    pos(segmentPos),
    slope(segmentSlope),
    height(letterHeight),
    font(fontSize),
    maxWid(maxWidth)
  {}

  bool intersect(Point2D point) const {
    if (point.x < pos.x || point.y < pos.y || fabs(point.x - pos.x) > maxWid) {
      return false;
    }

    float yIntercept = pos.y + height;
    float expY = slope * fabs(point.x - pos.x) + yIntercept;

    return fabs(expY - point.y) <= font;
  }

  Point2D pos;
  float slope;
  float height;
  float font;
  float maxWid;
};

#endif // !LETTER_SEGMENT_H
