#include "PPMImageFileWriter.h"

PPMImageFileWriter::PPMImageFileWriter(const PPMImage& image, std::string_view outputPath):
  PPMImageWriter(image), _outputPath(outputPath)
{}

void PPMImageFileWriter::write() const
{
  std::ofstream ppmFile;
  ppmFile.open(_outputPath.data());
  PPMImageWriter::write(ppmFile);
  ppmFile.close();
}
