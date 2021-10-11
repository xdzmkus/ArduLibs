#ifndef _GFX_FLOAT_H_
#define _GFX_FLOAT_H_

#include "internal/GFX_ValueObject.hpp"

class GFX_Float : public GFX_ValueObject<float>
{
public:

	GFX_Float(float value, uint8_t decimalPlaces, Adafruit_GFX* tft, uint8_t size, uint16_t background, uint16_t color, int16_t x, int16_t y, uint16_t w = 0, uint16_t h = 0)
		: GFX_ValueObject(value, tft, size, background, color, x, y, w, h), _decimalPlaces(decimalPlaces), _decimalSize(size)
	{
	};

	void setDecimalPlaces(uint8_t decimalPlaces, bool redraw = true);
	void setDecimalSize(uint8_t size, bool redraw = true);

protected:

	void setCursorSize() override;

	void draw(uint16_t color) override;

	const String toString() const override
	{
		return String(_value, _decimalPlaces);
	};

protected:

	uint8_t _decimalPlaces;
	uint8_t _decimalSize;
};

void GFX_Float::setDecimalPlaces(uint8_t decimalPlaces, bool redraw)
{
	if (redraw) clear();

	_decimalPlaces = decimalPlaces;

	if (redraw) show();
}

void GFX_Float::setDecimalSize(uint8_t size, bool redraw)
{
	if (redraw) clear();

	_decimalSize = (size >= _size) ? _size : size;

	if (redraw) show();
}

void GFX_Float::setCursorSize()
{
	_tft->setTextSize(_size);

	if (_w != 0 || _h != 0)
	{
		// to draw within rectangle or line

		int16_t x, y;
		uint16_t h1, w1;

		String strValue = toString();

		int dotIdx = strValue.indexOf('.');

		if (dotIdx >= 0 && _decimalSize != _size)
		{
			uint16_t h2, w2;
			_tft->setTextSize(_decimalSize);
			_tft->getTextBounds(strValue.substring(dotIdx), _x, _y, &x, &y, &w2, &h2);
			_tft->setTextSize(_size);
			_tft->getTextBounds(strValue.substring(0, dotIdx), _x, _y, &x, &y, &w1, &h1);
			w1 += w2;
			h1 += h2;
		}
		else
		{
			_tft->getTextBounds(strValue, _x, _y, &x, &y, &w1, &h1);
		}

		_tft->setCursor((_w == 0) ? _x : (_x + (_w - w1) / 2), (_h == 0) ? _y : (_y + (_h - h1) / 2));
	}
	else
	{
		// to draw just at x,y position
		_tft->setCursor(_x, _y);
	}
}

void GFX_Float::draw(uint16_t color)
{
	_tft->setTextColor(color);

	String strValue = toString();

	int dotIdx = strValue.indexOf('.');

	if (dotIdx >= 0 && _decimalSize != _size)
	{
		int16_t x1, y1;
		uint16_t h1, w1, h2, w2;

		String valuePart = strValue.substring(0, dotIdx);

		// print integer part

		_tft->getTextBounds(valuePart, _tft->getCursorX(), _tft->getCursorY(), &x1, &y1, &w1, &h1);

#ifdef DEBUG_DRAW
		_tft->drawRect(x1, y1, w1, h1, color);
		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", valuePart, w1, h1, x1, y1);
#endif
		_tft->print(valuePart);

		// print decimal part

		_tft->setTextSize(_decimalSize);

		valuePart = strValue.substring(dotIdx);

		_tft->getTextBounds(valuePart, _tft->getCursorX(), _tft->getCursorY(), &x1, &y1, &w2, &h2);

		y1 += h1 - h2;

		_tft->setCursor(x1, y1);

#ifdef DEBUG_DRAW
		_tft->drawRect(x1, y1, w2, h2, color);
		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", valuePart, w2, h2, x1, y1);
#endif
		_tft->print(valuePart);

		// print extension

		if (!_extension.isEmpty())
		{
			_tft->setTextSize(_ext_size);

			_tft->getTextBounds(_extension, x1, y1, &x1, &y1, &w1, &h1);

			_tft->setCursor(w1 >= w2 ? x1 : (x1 + w2 - w1), y1 - h1); // left or right adjust

#ifdef DEBUG_DRAW
			_tft->drawRect(_tft->getCursorX(), _tft->getCursorY(), w1, h1, color);
			Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", _extension, w1, h1, _tft->getCursorX(), _tft->getCursorY());
#endif
			_tft->print(_extension);
		}

		return;
	}

#ifdef DEBUG_DRAW
	int16_t x1, y1;
	uint16_t h1, w1;
	_tft->getTextBounds(strValue, _tft->getCursorX(), _tft->getCursorY(), &x1, &y1, &w1, &h1);
	_tft->drawRect(x1, y1, w1, h1, color);
	Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", strValue, w1, h1, x1, y1);
#endif

	_tft->print(strValue);

	if (!_extension.isEmpty())
	{
		_tft->setTextSize(_ext_size);

#ifdef DEBUG_DRAW
		_tft->getTextBounds(_extension, _tft->getCursorX(), _tft->getCursorY(), &x1, &y1, &w1, &h1);
		_tft->drawRect(x1, y1, w1, h1, color);
		Serial.printf("%s <=> width: %d and height: %d at X: %d and Y: %d\r\n", _extension, w1, h1, x1, y1);
#endif

		_tft->print(_extension);
	}
}

#endif
