#ifndef _GFX_TEXT_H_
#define _GFX_TEXT_H_

#include "internal/GFX_Object.h"

class GFX_Text : public GFX_Object
{
public:

	GFX_Text(String text, uint8_t size, Adafruit_GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: GFX_Object(tft, background, color, x, y, w, h), _text(text), _size(size)
	{
	};

	inline void setText(String text);
	inline void setSize(uint8_t size);

	inline void updateText(String text);

protected:

	virtual void setCursor() override;

	virtual void draw(uint16_t color) override;

protected:

	String _text;
	uint8_t _size;
};

inline void GFX_Text::setText(String text)
{
	_text = text;
}

inline void GFX_Text::setSize(uint8_t size)
{
	_size = size;
}

inline void GFX_Text::updateText(String text)
{
	if (_text == text) return;

	clear();

	_text = text;

	show();
}

void GFX_Text::setCursor()
{
	_tft->setTextSize(_size);

	if (_w != 0 || _h != 0)
	{
		uint16_t h1, w1;
		_tft->getTextBounds(_text, _x, _y, &_x, &_y, &w1, &h1);
		_tft->setCursor((_w == 0) ? _x : (_x + (_w - w1) / 2), (_h == 0) ? _y : (_y + (_h - h1) / 2));
	}
	else
	{
		_tft->setCursor(_x, _y);
	}
}

void GFX_Text::draw(uint16_t color)
{
	_tft->setTextColor(color);

#ifdef DEBUG_DRAW
	int16_t x1, y1;
	uint16_t h1, w1;
	_tft->getTextBounds(_text, _tft->getCursorX(), _tft->getCursorY(), &x1, &y1, &w1, &h1);
	_tft->drawRect(x1, y1, w1, h1, color);
	Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", _text, w1, h1, x1, y1);
#endif

	_tft->print(_text);
}

#endif
