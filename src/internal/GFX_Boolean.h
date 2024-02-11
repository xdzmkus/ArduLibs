#ifndef _GFX_BOOLEAN_H_
#define _GFX_BOOLEAN_H_

#include "GFX_Text.h"

namespace ArduLibs
{

	template <class GFX>
	class GFX_Boolean : public GFX_Text<GFX>
	{
	public:

		GFX_Boolean(bool value, String trueText, String falseText, uint8_t size, GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
			: GFX_Text<GFX>(value ? trueText : falseText, size, tft, background, color, x, y, w, h), _value(value), _trueText(trueText), _falseText(falseText)
		{
		};

		inline void updateValue(bool value);

	protected:

		bool _value;
		String _trueText;
		String _falseText;
	};


	template <class GFX>
	inline void GFX_Boolean<GFX>::updateValue(bool value)
	{
		if (_value == value) return;

		_value = value;

		GFX_Text<GFX>::updateText(value ? _trueText : _falseText);
	}

}

#endif

