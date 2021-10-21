#ifndef _GFX_UCHAR_H_
#define _GFX_UCHAR_H_

#include "internal/GFX_ValueObject.hpp"

class GFX_UChar : public GFX_ValueObject<unsigned char>
{
public:

	GFX_UChar(unsigned char ascii, Adafruit_GFX* tft, uint8_t size, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: GFX_ValueObject(ascii, tft, size, background, color, x, y, w, h)
	{
	};

protected:

	const String toString() const override
	{
		return String(static_cast<char>(_value));
	};
};

#endif
