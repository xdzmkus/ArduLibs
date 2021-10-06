#ifndef _GFX_OBJECT_H_
#define _GFX_OBJECT_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_GFX.h>

class GFX_Object
{
public:

	GFX_Object(Adafruit_GFX* tft, uint8_t size, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0);

	void setPosition(int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0, bool redraw = true);
	void setSize(uint8_t size, bool redraw = true);
	void setColors(uint16_t background, uint16_t color, bool redraw = true);
	void setExtension(String extension, uint8_t size, bool redraw = true);

	void clear();
	void show();

protected:

	virtual void setCursorSize();

	virtual void draw();

	virtual const String toString() const = 0;

protected:

	Adafruit_GFX* _tft;

	uint8_t _size;

	uint16_t _background;
	uint16_t _color;

	int16_t _x;
	int16_t _y;
	uint16_t _w;
	uint16_t _h;

	String  _extension;
	uint8_t _ext_size;
};


GFX_Object::GFX_Object(Adafruit_GFX* tft, uint8_t size, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w, uint16_t h)
	: _tft(tft), _size(size), _background(background), _color(color), _x(x), _y(y), _w(w), _h(h)
{
}

void GFX_Object::setCursorSize()
{
	_tft->setTextSize(_size);

	if (_w != 0 || _h != 0)
	{
		uint16_t h1, w1;
		_tft->getTextBounds(toString(), _x, _y, &_x, &_y, &w1, &h1);
		_tft->setCursor((_w == 0) ? _x : (_x + (_w - w1) / 2), (_h == 0) ? _y : (_y + (_h - h1) / 2));
	}
	else
	{
		_tft->setCursor(_x, _y);
	}
}

void GFX_Object::draw()
{
	_tft->print(toString());

	if (!_extension.isEmpty())
	{
		_tft->setTextSize(_ext_size);
		_tft->print(_extension);
	}
}

void GFX_Object::clear()
{
	_tft->setTextColor(_background);

	setCursorSize();

	draw();
}

void GFX_Object::show()
{
	_tft->setTextColor(_color);

	setCursorSize();

	draw();
}

void GFX_Object::setPosition(int16_t x, int16_t y, uint16_t w, uint16_t h, bool redraw)
{
	if (redraw) clear();

	_x = x;
	_y = y;
	_w = w;
	_h = h;

	if (redraw) show();
}

void GFX_Object::setSize(uint8_t size, bool redraw)
{
	if (redraw) clear();

	_size = size;

	if (redraw) show();
}

void GFX_Object::setColors(uint16_t background, uint16_t color, bool redraw)
{
	if (redraw) clear();

	_background = background;
	_color = color;

	if (redraw) show();
}

void GFX_Object::setExtension(String extension, uint8_t size, bool redraw)
{
	if (redraw) clear();

	_extension = extension;
	_ext_size = size;

	if (redraw) show();
}

#endif
