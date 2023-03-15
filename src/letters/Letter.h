#ifndef LETTER_H
#define LETTER_H

#include <vector>

#include "LetterSegment.h"
#include "utils/TypeDefs.h"

struct Letter {
  Letter() = default;
  Letter(Point2D pos, const std::vector<LetterSegment>& segments) :
    _pos(pos),
    _segs(segments)
  {}

  bool intersect(Point2D point) {
    for (auto& seg : _segs) {
      if (seg.intersect(point)) {
        return true;
      }
    }
    return false;
  }

  void setPosition(Point2D pos) {
    _pos = pos;

    for (auto& seg : _segs) {
      seg.pos += pos;
    }
  }
private:
  Point2D _pos;
  std::vector<LetterSegment> _segs;
};

#endif // !LETTER_H
