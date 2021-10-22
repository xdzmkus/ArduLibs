#ifndef _GFX_UCHAR_H_
#define _GFX_UCHAR_H_

#include "internal/GFX_ValueObject.hpp"

class GFX_UChar : public GFX_ValueObject<unsigned char>
{
public:

	GFX_UChar(unsigned char ascii, Adafruit_GFX* tft, uint8_t size, uint16_t background, uint16_t color, int16_t x, int16_t y)
		: GFX_ValueObject(ascii, tft, size, background, color, x, y, 0, 0)
	{
	};

	GFX_UChar(unsigned char ascii, Adafruit_GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w, uint16_t h)
		: GFX_ValueObject(ascii, tft, 0, background, color, x, y, w, h)
	{
	};

protected:

	void setCursorSize() override;

	void draw(uint16_t color) override;

	const String toString() const override
	{
		return String(static_cast<char>(_value));
	};
};


void GFX_UChar::setCursorSize()
{
	int16_t x1, y1;
	uint16_t h1, w1;

	_tft->setTextSize(_size != 0 ? _size : _w);
	_tft->getTextBounds(toString(), _x, _y, &x1, &y1, &w1, &h1);
	_tft->setCursor(x1 + w1, y1);
}

void GFX_UChar::draw(uint16_t color)
{
	_tft->setTextColor(color);

	if (_size != 0)
	{

#ifdef DEBUG_DRAW
		Serial.printf("char %s <=> size: %d at X: %d and Y: %d\r\n", toString(), _size, _x, _y);
#endif

		_tft->drawChar(_x, _y, _value, color, _background, _size);
	}
	else
	{
#ifdef DEBUG_DRAW
		Serial.printf("char %s <=> width: %d and height: %d at X: %d and Y: %d\r\n", toString(), _w, _h, _x, _y);
#endif

		_tft->drawChar(_x, _y, _value, color, _background, _w, _h);
	}

	if (!_extension.isEmpty())
	{
		_tft->setTextSize(_ext_size);

#ifdef DEBUG_DRAW
		int16_t x1, y1;
		uint16_t h1, w1;
		_tft->getTextBounds(_extension, _tft->getCursorX(), _tft->getCursorY(), &x1, &y1, &w1, &h1);
		_tft->drawRect(x1, y1, w1, h1, color);
		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", _extension, w1, h1, x1, y1);
#endif

		_tft->print(_extension);
	}

}
#endif
