#include "PPMImage.h"

#include <assert.h>

PPMImage::PPMImage(const PPMImageMeta& metadata): _metadata(metadata), _currentRow(0)
{
  _image.reserve(metadata._imgHeight);
}

void PPMImage::writePixel(const Color& color)
{
  if (_currentRow >= _metadata._imgHeight) {
    assert("Buffer is full.");
    return;
  }
  if (_image.empty()) {
    _image.push_back({});
  }
  if (_image[_currentRow].size() == 0) {
    _image[_currentRow].reserve(_metadata._imgWidth);
  }

  _image[_currentRow].push_back(color);

  if (_image[_currentRow].size() == _metadata._imgWidth) {
    _currentRow++;
    _image.push_back({});
  }
}
