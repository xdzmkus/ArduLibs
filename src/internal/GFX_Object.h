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

	GFX_Object(Adafruit_GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: _tft(tft), _background(background), _color(color), _x(x), _y(y), _w(w), _h(h)
	{
	};

	inline void setDimensions(int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0);
	inline void setColors(uint16_t background, uint16_t color);

	void clear();
	void show();

	void fill();

protected:

	virtual void setCursor() = 0;

	virtual void draw(uint16_t color) = 0;

protected:

	Adafruit_GFX* _tft;

	uint16_t _background;
	uint16_t _color;

	int16_t _x;
	int16_t _y;
	uint16_t _w;
	uint16_t _h;
};

void GFX_Object::clear()
{
#ifdef DEBUG_DRAW
	_tft->drawRect(_x, _y, _w, _h, _background);
#endif
		
	setCursor();

	draw(_background);
}

void GFX_Object::show()
{
#ifdef DEBUG_DRAW
	_tft->drawRect(_x, _y, _w, _h, _color);
#endif

	setCursor();

	draw(_color);
}

void GFX_Object::fill()
{
	_tft->fillRect(_x, _y, _w, _h, _background);
}

inline void GFX_Object::setDimensions(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

inline void GFX_Object::setColors(uint16_t background, uint16_t color)
{
	_background = background;
	_color = color;
}

#endif
