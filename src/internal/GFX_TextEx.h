#ifndef _GFX_TEXTEX_H_
#define _GFX_TEXTEX_H_

#include "GFX_Text.h"

namespace ArduLibs
{

	template <class GFX>
	class GFX_TextEx : public GFX_Text<GFX>
	{
	public:

		GFX_TextEx(String text, uint8_t size, String extText, uint8_t extSize, GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
			: GFX_Text<GFX>(text, size, tft, background, color, x, y, w, h), _extension(extText), _extensionSize(extSize)
		{
		};

		inline void setExtension(String extText);
		inline void setExtensionSize(uint8_t extSize);

	protected:

		virtual void draw(uint16_t color) override;

	protected:

		String _extension;
		uint8_t _extensionSize;
	};

	template <class GFX>
	inline void GFX_TextEx<GFX>::setExtension(String extText)
	{
		_extension = extText;
	}

	template <class GFX>
	inline void GFX_TextEx<GFX>::setExtensionSize(uint8_t extSize)
	{
		_extensionSize = extSize;
	}

	template <class GFX>
	void GFX_TextEx<GFX>::draw(uint16_t color)
	{
		GFX_Text<GFX>::draw(color);

		if (!_extension.isEmpty())
		{
			this->_tft->setTextSize(_extensionSize);

#ifdef DEBUG_DRAW
			int16_t x1, y1;
			uint16_t h1, w1;
			this->_tft->getTextBounds(_extension, this->_tft->getCursorX(), this->_tft->getCursorY(), &x1, &y1, &w1, &h1);
			this->_tft->drawRect(x1, y1, w1, h1, color);
			//		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", _extension, w1, h1, x1, y1);
#endif

			this->_tft->print(_extension);
		}
	}

}

#endif
