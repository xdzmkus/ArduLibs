#ifndef _GFX_OBJECT_H_
#define _GFX_OBJECT_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace ArduLibs
{

	template <class GFX>
	class GFX_Object
	{
	public:

		GFX_Object(GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
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

		GFX* _tft;

		uint16_t _background;
		uint16_t _color;

		int16_t _x;
		int16_t _y;
		uint16_t _w;
		uint16_t _h;
	};

	template <class GFX>
	void GFX_Object<GFX>::clear()
	{
#ifdef DEBUG_DRAW
		_tft->drawRect(_x, _y, _w, _h, _background);
#endif

		setCursor();

		draw(_background);
	}

	template <class GFX>
	void GFX_Object<GFX>::show()
	{
#ifdef DEBUG_DRAW
		_tft->drawRect(_x, _y, _w, _h, _color);
#endif

		setCursor();

		draw(_color);
	}

	template <class GFX>
	void GFX_Object<GFX>::fill()
	{
		_tft->fillRect(_x, _y, _w, _h, _background);
	}

	template <class GFX>
	inline void GFX_Object<GFX>::setDimensions(int16_t x, int16_t y, uint16_t w, uint16_t h)
	{
		_x = x;
		_y = y;
		_w = w;
		_h = h;
	}

	template <class GFX>
	inline void GFX_Object<GFX>::setColors(uint16_t background, uint16_t color)
	{
		_background = background;
		_color = color;
	}

}

#endif
