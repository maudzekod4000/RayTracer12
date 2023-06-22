#include "PPMImage.h"

PPMImage::PPMImage(const PPMImageMeta& metadata): _metadata(metadata), _currentRow(0)
{
  _image.resize(metadata._imgHeight);

  for (int32_t i = 0; i < metadata._imgHeight; i++) {
    _image[i].resize(metadata._imgWidth);
  }
}

void PPMImage::writePixel(int32_t row, int32_t col, const PPMColor& color)
{
  _image[row][col] = color;
}
