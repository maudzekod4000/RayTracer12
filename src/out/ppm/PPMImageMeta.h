#ifndef PPM_IMAGE_META_H
#define PPM_IMAGE_META_H

#include <string>
#include <ostream>
#include <stdint.h>

/// <summary>
/// Holds metadata of a PPM output image.
/// Has methods for writing data to an output stream.
/// </summary>
class PPMImageMeta {
public:
	PPMImageMeta(
		uint16_t imgWidth,
		uint16_t imgHeight,
		uint8_t maxColor
	);

	/// <summary>
	/// Given an output stream it writes instance fields to it.
	/// </summary>
	/// <param name="os">Output stream to write to.</param>
	void writeHeaders(std::ostream& os);
private:
	std::string colorFormat;
	uint16_t imgWidth;
	uint16_t imgHeight;
	uint8_t maxColor;
};

#endif // !PPM_IMAGE_META_H
