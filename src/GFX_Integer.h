#ifndef _GFX_INTEGER_H_
#define _GFX_INTEGER_H_

#include "internal/GFX_ValueObject.hpp"

class GFX_Integer : public GFX_ValueObject<int16_t>
{
public:

	GFX_Integer(int16_t value, Adafruit_GFX* tft, uint8_t size, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: GFX_ValueObject(value, tft, size, background, color, x, y, w, h)
	{
	};

protected:

	const String toString() const override
	{
		return String(_value);
	};
};

#endif

