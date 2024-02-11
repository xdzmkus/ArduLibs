#ifndef _GFX_UCHAR_H_
#define _GFX_UCHAR_H_

#include "internal/GFX_Object.h"

namespace ArduLibs
{

	template <class GFX>
	class GFX_UChar : public GFX_Object<GFX>
	{
	public:

		GFX_UChar(unsigned char ascii, uint8_t size, GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y)
			: GFX_Object<GFX>(tft, background, color, x, y, 0, 0), _value(ascii), _size(size)
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

	template <class GFX>
	inline void GFX_UChar<GFX>::updateValue(unsigned char ascii)
	{
		if (_value == ascii) return;

		this->clear();

		_value = ascii;

		this->show();
	}

	template <class GFX>
	inline void GFX_UChar<GFX>::setCursor()
	{
		this->_tft->setCursor(this->_x, this->_y);
	}

	template <class GFX>
	void GFX_UChar<GFX>::draw(uint16_t color)
	{
#ifdef DEBUG_DRAW
			//		Serial.printf("char %c <=> size: %d at X: %d and Y: %d\r\n", _value, this->_size, this->_x, this->_y);
#endif
		this->_tft->drawChar(this->_x, this->_y, _value, color, this->_background, this->_size);
	}

}

#endif
