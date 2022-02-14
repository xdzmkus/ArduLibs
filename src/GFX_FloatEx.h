#ifndef _GFX_FLOATEX_H_
#define _GFX_FLOATEX_H_

#include "GFX_TextEx.h"

class GFX_FloatEx : public GFX_TextEx
{
public:

	GFX_FloatEx(float value, uint8_t decimalPlaces, uint8_t size, String extText, uint8_t extSize, Adafruit_GFX* tft, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: GFX_TextEx(String(value, decimalPlaces), size, extText, extSize, tft, background, color, x, y, w, h), _value(value), _decimalPlaces(decimalPlaces), _decimalSize(size)
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

inline void GFX_FloatEx::setDecimalPlaces(uint8_t decimalPlaces)
{
	_decimalPlaces = decimalPlaces;
}

inline void GFX_FloatEx::setDecimalSize(uint8_t size)
{
	_decimalSize = (size >= _size) ? _size : size;
}

inline void GFX_FloatEx::updateValue(float value)
{
	if (_value == value) return;

	_value = value;

	GFX_TextEx::updateText(String(value, _decimalPlaces));
}

void GFX_FloatEx::setCursor()
{
	_tft->setTextSize(_size);

	if (_w != 0 || _h != 0)
	{
		// to draw within rectangle or line

		int16_t x, y;
		uint16_t h1, w1;

		int dotIdx = _text.indexOf('.');

		if (dotIdx >= 0 && _decimalSize != _size)
		{
			uint16_t h2, w2;
			_tft->setTextSize(_decimalSize);
			_tft->getTextBounds(_text.substring(dotIdx), _x, _y, &x, &y, &w2, &h2);
			_tft->setTextSize(_size);
			_tft->getTextBounds(_text.substring(0, dotIdx), _x, _y, &x, &y, &w1, &h1);
			w1 += w2;
			if (h2 > h1) h1 = h2;
		}
		else
		{
			_tft->getTextBounds(_text, _x, _y, &x, &y, &w1, &h1);
		}

		_tft->setCursor((_w == 0) ? _x : (_x + (_w - w1) / 2), (_h == 0) ? _y : (_y + (_h - h1) / 2));
	}
	else
	{
		// to draw just at x,y position
		_tft->setCursor(_x, _y);
	}
}

void GFX_FloatEx::draw(uint16_t color)
{
	int dotIdx = _text.indexOf('.');

	if (dotIdx >= 0 && _decimalSize != _size)
	{
		int16_t x1, y1;
		uint16_t h1, w1, h2, w2;

		String valuePart = _text.substring(0, dotIdx);

		_tft->setTextColor(color);

		// print integer part

		_tft->getTextBounds(valuePart, _tft->getCursorX(), _tft->getCursorY(), &x1, &y1, &w1, &h1);

#ifdef DEBUG_DRAW
		_tft->drawRect(x1, y1, w1, h1, color);
		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", valuePart, w1, h1, x1, y1);
#endif
		_tft->print(valuePart);

		// print decimal part

		_tft->setTextSize(_decimalSize);

		valuePart = _text.substring(dotIdx);

		_tft->getTextBounds(valuePart, _tft->getCursorX(), _tft->getCursorY(), &x1, &y1, &w2, &h2);

		y1 += h1 - h2;

		_tft->setCursor(x1, y1);

#ifdef DEBUG_DRAW
		_tft->drawRect(x1, y1, w2, h2, color);
		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", valuePart, w2, h2, x1, y1);
#endif
		_tft->print(valuePart);

		// print extension

		if (!extension.isEmpty())
		{
			_tft->setTextSize(extensionSize);

			_tft->getTextBounds(extension, x1, y1, &x1, &y1, &w1, &h1);

			_tft->setCursor(w1 >= w2 ? x1 : (x1 + w2 - w1), y1 - h1); // left or right adjust

#ifdef DEBUG_DRAW
			_tft->drawRect(_tft->getCursorX(), _tft->getCursorY(), w1, h1, color);
			Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", extension, w1, h1, _tft->getCursorX(), _tft->getCursorY());
#endif
			_tft->print(extension);
		}
	}
	else
	{
		GFX_TextEx::draw(color);
	}
}

#endif
