#pragma once

#include <string>
#include <ostream>
#include <stdint.h>

/// <summary>
/// Holds metadata of a PPM output image.
/// </summary>
class PPMImage {
public:
	PPMImage(
		unsigned short imgWidth,
		unsigned short imgHeight,
		unsigned short maxColor
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
	uint16_t maxColor;
};
