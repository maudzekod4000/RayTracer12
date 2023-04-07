#include "PPMImageWriter.h"

#include "PPMImageMeta.h"

#include <fstream>
#include <string>

PPMImageWriter::PPMImageWriter(const PPMImage& image): _imageWrapper(image)
{
}

__declspec(safebuffers)
void PPMImageWriter::write(std::ostream& output) const
{
  // Write the headers first
  _imageWrapper._metadata.writeHeaders(output);

  std::string buffer;
  // Each color takes maximum of '255 255 255' -> 11 characters and there is a space after each color so 12.
  buffer.reserve(12 * _imageWrapper._image.size());
  for (auto& row : _imageWrapper._image) {
    for (auto& color : row) {
      buffer += std::to_string(color.r) + " " + std::to_string(color.g) + " " + std::to_string(color.b) + " ";
      /*buffer += " ";
      buffer += std::to_string(color.g);
      buffer += " ";
      buffer += std::to_string(color.b);
      buffer += " ";*/
    }
    buffer += '\n';
  }

  output << buffer;
}
