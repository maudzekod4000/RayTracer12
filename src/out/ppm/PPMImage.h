#ifndef PPM_IMAGE_H
#define PPM_IMAGE_H

#include <vector>

#include "PPMImageMeta.h"
#include "PPMColor.h"
#include "utils/TypeDefs.h"

/// <summary>
/// In-memory representation of an image in the PPM format.
/// Can be used for displaying in a frame buffer or post-processing.
/// </summary>
class PPMImage {
public:
  friend class PPMImageWriter;
  PPMImage(const PPMImageMeta& metadata);

  void writePixel(int32_t row, int32_t col, const PPMColor& color);
protected:
  std::vector<std::vector<PPMColor>> _image;
  const PPMImageMeta& _metadata;
  int32_t _currentRow;
};

#endif // !PPM_IMAGE_H
