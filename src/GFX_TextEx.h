#ifndef _GFX_TEXTEX_H_
#define _GFX_TEXTEX_H_

#include "GFX_Text.h"

class GFX_TextEx : public GFX_Text
{
public:

	GFX_TextEx(String text, uint8_t size, String extText, uint8_t extSize, Adafruit_GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: GFX_Text(text, size, tft, background, color, x, y, w, h), extension(extText), extensionSize(extSize)
	{
	};

	inline void setExtension(String extText);
	inline void setExtensionSize(uint8_t extSize);

protected:

	virtual void draw(uint16_t color) override;

protected:

	String extension;
	uint8_t extensionSize;
};

inline void GFX_TextEx::setExtension(String extText)
{
	extension = extText;
}

inline void GFX_TextEx::setExtensionSize(uint8_t extSize)
{
	extensionSize = extSize;
}

void GFX_TextEx::draw(uint16_t color)
{
	GFX_Text::draw(color);

	if (!extension.isEmpty())
	{
		_tft->setTextSize(extensionSize);

#ifdef DEBUG_DRAW
		int16_t x1, y1;
		uint16_t h1, w1;
		_tft->getTextBounds(extension, _tft->getCursorX(), _tft->getCursorY(), &x1, &y1, &w1, &h1);
		_tft->drawRect(x1, y1, w1, h1, color);
		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", extension, w1, h1, x1, y1);
#endif

		_tft->print(extension);
	}
}

#endif
