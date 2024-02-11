#ifndef _GFX_TEXT_H_
#define _GFX_TEXT_H_

#include "internal/GFX_Object.h"

namespace ArduLibs
{

	template <class GFX>
	class GFX_Text : public GFX_Object<GFX>
	{
	public:

		GFX_Text(String text, uint8_t size, GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
			: GFX_Object<GFX>(tft, background, color, x, y, w, h), _text(text), _size(size)
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

	template <class GFX>
	inline void GFX_Text<GFX>::setText(String text)
	{
		_text = text;
	}

	template <class GFX>
	inline void GFX_Text<GFX>::setSize(uint8_t size)
	{
		_size = size;
	}

	template <class GFX>
	inline void GFX_Text<GFX>::updateText(String text)
	{
		if (_text == text) return;

		GFX_Object<GFX>::clear();

		_text = text;

		GFX_Object<GFX>::show();
	}

	template <class GFX>
	void GFX_Text<GFX>::setCursor()
	{
		this->_tft->setTextSize(_size);

		if (this->_w != 0 || this->_h != 0)
		{
			uint16_t h1, w1;
			this->_tft->getTextBounds(_text, this->_x, this->_y, &this->_x, &this->_y, &w1, &h1);
			this->_tft->setCursor((this->_w == 0) ? this->_x : (this->_x + (this->_w - w1) / 2), (this->_h == 0) ? this->_y : (this->_y + (this->_h - h1) / 2));
		}
		else
		{
			this->_tft->setCursor(this->_x, this->_y);
		}
	}

	template <class GFX>
	void GFX_Text<GFX>::draw(uint16_t color)
	{
		this->_tft->setTextColor(color);

#ifdef DEBUG_DRAW
		int16_t x1, y1;
		uint16_t h1, w1;
		this->_tft->getTextBounds(_text, this->_tft->getCursorX(), this->_tft->getCursorY(), &x1, &y1, &w1, &h1);
		this->_tft->drawRect(x1, y1, w1, h1, color);
		//	Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", _text, w1, h1, x1, y1);
#endif

		this->_tft->print(_text);
	}

}

#endif
