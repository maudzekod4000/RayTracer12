#include "PPMImageWriter.h"

#include "PPMImageMeta.h"

#include <fstream>

PPMImageWriter::PPMImageWriter(const PPMImage& image): _imageWrapper(image)
{
}

void PPMImageWriter::write(std::ostream& output) const
{
  _imageWrapper._metadata.writeHeaders(output);

  for (auto& row : _imageWrapper._image) {
    for (auto& color : row) {
      output << color.r << " " << color.g << " " << color.b << " ";
    }
    output << std::endl;
  }
}
