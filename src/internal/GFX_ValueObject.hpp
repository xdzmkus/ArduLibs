#ifndef _GFX_VALUEOBJECT_HPP_
#define _GFX_VALUEOBJECT_HPP_

#include "GFX_Object.h"

template <class T>
class GFX_ValueObject : public GFX_Object
{
public:

	GFX_ValueObject(T value, Adafruit_GFX* tft, uint8_t size, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0);

	void setValue(T value, bool redraw = true);

protected:

	T _value;
};

template <class T>
GFX_ValueObject<T>::GFX_ValueObject(T value, Adafruit_GFX* tft, uint8_t size, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w, uint16_t h)
	: GFX_Object(tft, size, background, color, x, y, w, h), _value(value)
{
}

template<class T>
void GFX_ValueObject<T>::setValue(T value, bool redraw)
{
	if (_value == value) return;

	if (redraw) clear();

	_value = value;

	if (redraw)	show();
}

#endif
