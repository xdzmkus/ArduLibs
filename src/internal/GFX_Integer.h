#ifndef _GFX_INTEGER_H_
#define _GFX_INTEGER_H_

#include "GFX_Text.h"

namespace ArduLibs
{

	template <class GFX>
	class GFX_Integer : public GFX_Text<GFX>
	{
	public:

		GFX_Integer(int16_t value, uint8_t size, GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
			: GFX_Text<GFX>(String(value), size, tft, background, color, x, y, w, h), _value(value)
		{
		};

		inline void updateValue(int16_t value);

	protected:

		int16_t _value;
	};


	template <class GFX>
	inline void GFX_Integer<GFX>::updateValue(int16_t value)
	{
		if (_value == value) return;

		_value = value;

		GFX_Text<GFX>::updateText(String(value));
	}

}

#endif

