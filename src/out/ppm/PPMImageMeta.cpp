#include "PPMImageMeta.h"

PPMImageMeta::PPMImageMeta(
	uint16_t imgWidth,
	uint16_t imgHeight,
	uint16_t maxColor
): _colorFormat("P3"), _imgWidth(imgWidth), _imgHeight(imgHeight), _maxColor(maxColor) {}

// This method is called only once per image, so it is okay to use ostream's <<
// instead of string's '+' operator, even though it is less efficient.
void PPMImageMeta::writeHeaders(std::ostream& os) const {
	os << _colorFormat << std::endl
		<< _imgWidth << " " << _imgHeight << std::endl
		<< std::to_string(_maxColor) << std::endl;
}
