#ifndef LETTER_FACTORY_H
#define LETTER_FACTORY_H

#include <unordered_map>
#include <vector>

#include "Letter.h"
#include "LetterSegment.h"

struct LetterFactory {
  LetterFactory() {
    LetterSegment ascRight(Point2D(0, 0), -5.0f, 60, 20, 1000);
    LetterSegment descRight(Point2D(15, 0), 5.0f, 0, 20, 12.5);
    LetterSegment strLine(Point2D(0, 40), 0, 0, 10, 22.5);
    // Constr. letter A
    std::vector<LetterSegment> segmentsForA = {
      ascRight,
      descRight,
      strLine
    };
    
    letters['A'] = Letter(Point2D(0, 0), segmentsForA);

    // Define the letter V
    LetterSegment leftStick = descRight;
    leftStick.pos = Point2D();
    LetterSegment rightStick = ascRight;
    rightStick.pos = Point2D(15, 0);
    std::vector<LetterSegment> segmentsForV = {
      leftStick, rightStick
    };

    letters['V'] = Letter(Point2D(0, 0), segmentsForV);

    // Def letter R
    LetterSegment straighStick(Point2D(0, 0), -30, 60, 30, 1000);
    LetterSegment topHalfCircle(Point2D(5, 0), 1, 10, 10, 12.5);
    LetterSegment botHalfCircle(Point2D(5, 0), -1, 60, 10, 12.5);
    LetterSegment leg(Point2D(5, 0), 1, 70, 10, 12.5);

    letters['R'] = Letter(Point2D(0, 0), { straighStick, topHalfCircle, botHalfCircle, leg });

    // Def Y
    LetterSegment hand(Point2D(0, 0), 1, 0, 10, 22.5);
    LetterSegment legger(Point2D(0, 0), -1, 60, 10, 52.5);

    letters['Y'] = Letter(Point2D(0, 0), { hand, legger });
  }

  std::unordered_map<char, Letter> letters;
};

#endif // !LETTER_FACTORY_H
