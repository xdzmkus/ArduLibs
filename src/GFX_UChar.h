#ifndef _GFX_UCHAR_H_
#define _GFX_UCHAR_H_

#include "internal/GFX_Object.h"

class GFX_UChar : public GFX_Object
{
public:

	GFX_UChar(unsigned char ascii, uint8_t size, Adafruit_GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y)
		: GFX_Object(tft, background, color, x, y, 0, 0), _value(ascii), _size(size)
	{
	};

	GFX_UChar(unsigned char ascii, Adafruit_GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w, uint16_t h)
		: GFX_Object(tft, background, color, x, y, w, h), _value(ascii), _size(0)
	{
	};

	inline void updateValue(unsigned char ascii);

protected:

	inline void setCursor() override;

	void draw(uint16_t color) override;

protected:

	unsigned char _value;
	uint8_t _size;
};

inline void GFX_UChar::updateValue(unsigned char ascii)
{
	if (_value == ascii) return;

	clear();

	_value = ascii;

	show();
}

inline void GFX_UChar::setCursor()
{
	_tft->setCursor(_x, _y);
}

void GFX_UChar::draw(uint16_t color)
{
	if (_size != 0)
	{

#ifdef DEBUG_DRAW
//		Serial.printf("char %c <=> size: %d at X: %d and Y: %d\r\n", _value, _size, _x, _y);
#endif

		_tft->drawChar(_x, _y, _value, color, _background, _size);
	}
	else
	{
#ifdef DEBUG_DRAW
//		Serial.printf("char %c <=> width: %d and height: %d at X: %d and Y: %d\r\n", _value, _w, _h, _x, _y);
#endif

		_tft->drawChar(_x, _y, _value, color, _background, _w, _h);
	}
}

#endif
