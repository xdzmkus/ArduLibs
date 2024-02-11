#ifndef _GFX_FLOAT_H_
#define _GFX_FLOAT_H_

#include "GFX_Text.h"

namespace ArduLibs
{

	template <class GFX>
	class GFX_Float : public GFX_Text<GFX>
	{
	public:

		GFX_Float(float value, uint8_t decimalPlaces, uint8_t size, GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
			: GFX_Text<GFX>(String(value, decimalPlaces), size, tft, background, color, x, y, w, h), _value(value), _decimalPlaces(decimalPlaces), _decimalSize(size)
		{
		};

		inline void setDecimalPlaces(uint8_t decimalPlaces);
		inline void setDecimalSize(uint8_t size);

		inline void updateValue(float value);

	protected:

		void setCursor() override;

		void draw(uint16_t color) override;

	protected:

		float _value;

		uint8_t _decimalPlaces;
		uint8_t _decimalSize;
	};

	template <class GFX>
	inline void GFX_Float<GFX>::setDecimalPlaces(uint8_t decimalPlaces)
	{
		_decimalPlaces = decimalPlaces;
	}

	template <class GFX>
	inline void GFX_Float<GFX>::setDecimalSize(uint8_t size)
	{
		_decimalSize = (size >= this->_size) ? this->_size : size;
	}

	template <class GFX>
	inline void GFX_Float<GFX>::updateValue(float value)
	{
		if (_value == value) return;

		_value = value;

		GFX_Text<GFX>::updateText(String(value, _decimalPlaces));
	}

	template <class GFX>
	void GFX_Float<GFX>::setCursor()
	{
		this->_tft->setTextSize(this->_size);

		if (this->_w != 0 || this->_h != 0)
		{
			// to draw within rectangle or line

			int16_t x, y;
			uint16_t h1, w1;

			int dotIdx = this->_text.indexOf('.');

			if (dotIdx >= 0 && _decimalSize != this->_size)
			{
				uint16_t h2, w2;
				this->_tft->setTextSize(_decimalSize);
				this->_tft->getTextBounds(this->_text.substring(dotIdx), this->_x, this->_y, &x, &y, &w2, &h2);
				this->_tft->setTextSize(this->_size);
				this->_tft->getTextBounds(this->_text.substring(0, dotIdx), this->_x, this->_y, &x, &y, &w1, &h1);
				w1 += w2;
				if (h2 > h1) h1 = h2;
			}
			else
			{
				this->_tft->getTextBounds(this->_text, this->_x, this->_y, &x, &y, &w1, &h1);
			}

			this->_tft->setCursor((this->_w == 0) ? this->_x : (this->_x + (this->_w - w1) / 2), (this->_h == 0) ? this->_y : (this->_y + (this->_h - h1) / 2));
		}
		else
		{
			// to draw just at x,y position
			this->_tft->setCursor(this->_x, this->_y);
		}
	}

	template <class GFX>
	void GFX_Float<GFX>::draw(uint16_t color)
	{
		int dotIdx = this->_text.indexOf('.');

		if (dotIdx >= 0 && _decimalSize != this->_size)
		{
			int16_t x1, y1;
			uint16_t h1, w1, h2, w2;

			String valuePart = this->_text.substring(0, dotIdx);

			this->_tft->setTextColor(color);

			// print integer part

			this->_tft->getTextBounds(valuePart, this->_tft->getCursorX(), this->_tft->getCursorY(), &x1, &y1, &w1, &h1);

#ifdef DEBUG_DRAW
			this->_tft->drawRect(x1, y1, w1, h1, color);
			//		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", valuePart, w1, h1, x1, y1);
#endif
			this->_tft->print(valuePart);

			// print decimal part

			this->_tft->setTextSize(_decimalSize);

			valuePart = this->_text.substring(dotIdx);

			this->_tft->getTextBounds(valuePart, this->_tft->getCursorX(), this->_tft->getCursorY(), &x1, &y1, &w2, &h2);

			y1 += h1 - h2;

			this->_tft->setCursor(x1, y1);

#ifdef DEBUG_DRAW
			this->_tft->drawRect(x1, y1, w2, h2, color);
			//		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", valuePart, w2, h2, x1, y1);
#endif
			this->_tft->print(valuePart);
		}
		else
		{
			GFX_Text<GFX>::draw(color);
		}
	}

}

#endif
