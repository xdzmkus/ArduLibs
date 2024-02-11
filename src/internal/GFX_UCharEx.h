#ifndef _GFX_UCHAREX_H_
#define _GFX_UCHAREX_H_

#include "GFX_TextEx.h"

namespace ArduLibs
{

	template <class GFX>
	class GFX_UCharEx : public GFX_TextEx<GFX>
	{
	public:

		GFX_UCharEx(unsigned char ascii, uint8_t size, String extText, uint8_t extSize, GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
			: GFX_TextEx<GFX>(String(static_cast<char>(ascii)), size, extText, extSize, tft, background, color, x, y, w, h), _value(ascii)
		{
		};

		inline void updateValue(unsigned char ascii);

	protected:

		unsigned char _value;
	};


	template <class GFX>
	inline void GFX_UCharEx<GFX>::updateValue(unsigned char ascii)
	{
		if (_value == ascii) return;

		_value = ascii;

		GFX_TextEx<GFX>::updateText(String(static_cast<char>(ascii)));
	}

}

#endif
