#ifndef _GFX_INTEGEREX_H_
#define _GFX_INTEGEREX_H_

#include "GFX_TextEx.h"

class GFX_IntegerEx : public GFX_TextEx
{
public:

	GFX_IntegerEx(int16_t value, uint8_t size, String extText, uint8_t extSize, Adafruit_GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: GFX_TextEx(String(value), size, extText, extSize, tft, background, color, x, y, w, h), _value(value)
	{
	};

	inline void updateValue(int16_t value);

protected:

	int16_t _value;
};


inline void GFX_IntegerEx::updateValue(int16_t value)
{
	if (_value == value) return;

	_value = value;

	GFX_TextEx::updateText(String(value));
}

#endif

