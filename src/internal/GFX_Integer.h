#ifndef _GFX_INTEGER_H_
#define _GFX_INTEGER_H_

#include "GFX_Text.h"

class GFX_Integer : public GFX_Text
{
public:

	GFX_Integer(int16_t value, uint8_t size, Adafruit_GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: GFX_Text(String(value), size, tft, background, color, x, y, w, h), _value(value)
	{
	};

	inline void updateValue(int16_t value);

protected:

	int16_t _value;
};


inline void GFX_Integer::updateValue(int16_t value)
{
	if (_value == value) return;

	_value = value;

	GFX_Text::updateText(String(value));
}

#endif

