#ifndef _GFX_STRING_H_
#define _GFX_STRING_H_

#include "internal/GFX_ValueObject.hpp"

class GFX_String : public GFX_ValueObject<String>
{
public:

	GFX_String(String text, Adafruit_GFX* tft, uint8_t size, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: GFX_ValueObject(text, tft, size, background, color, x, y, w, h)
	{
	};

protected:

	const String toString() const override
	{
		return _value;
	};
};

#endif
