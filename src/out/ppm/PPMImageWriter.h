#ifndef PPM_IMAGE_WRITER_H
#define PPM_IMAGE_WRITER_H

#include <fstream>

#include "PPMImage.h"

class PPMImageWriter {
public:
  PPMImageWriter(const PPMImage& image);

  void write(std::ostream& output) const;
private:
  const PPMImage& _imageWrapper;
};

#endif // !PPM_IMAGE_WRITER_H
