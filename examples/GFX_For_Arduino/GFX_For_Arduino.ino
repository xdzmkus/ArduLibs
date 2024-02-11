#define DEBUG_OUTPUT
#define DEBUG_DRAW

#ifndef DEBUG_OUTPUT
#define log_print(msg)
#define log_println(msg)
#else
#define log_print(msg) Serial.print(msg)
#define log_println(msg) Serial.println(msg)
#endif

#include "GFX.h"

using namespace ArduLibs;

// *** TFT-1.4 *** 128 x 128 ***//

static const uint16_t screenWidth = 128;
static const uint16_t screenHeight = 128;
static const uint8_t rotation = 0;

#include <Arduino_GFX_Library.h>

#define TFT_MISO GFX_NOT_DEFINED
#define TFT_MOSI D7
#define TFT_SCLK D5
#define TFT_CS   D4   // Chip select control pin
#define TFT_DC   D3   // Data Command control pin
#define TFT_RST  GFX_NOT_DEFINED // Reset pin (could connect to RST pin)

class Arduino_ST7735_Ex : public Arduino_ST7735
{
public:

	Arduino_ST7735_Ex(Arduino_DataBus* bus, int8_t rst, uint8_t r, bool ips, int16_t w, int16_t h) : Arduino_ST7735(bus, rst, r, ips, w, h)
	{
	};

	void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
	{
		this->setTextSize(size);
		Arduino_ST7735::drawChar(x, y, c, color, bg);
	};
};

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus* bus = new Arduino_ESP8266SPI(TFT_DC /* DC */, TFT_CS /* CS */);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_ST7735_Ex* tft = new Arduino_ST7735_Ex(bus, TFT_RST /* RST */, rotation /* rotation */, false /* IPS */, screenWidth /* width */, screenHeight /* height */);

unsigned int background = WHITE;
unsigned int color = BLACK;

unsigned char charLCD = 0;

#define TEMP_X 0
#define TEMP_Y 4
#define TEMP_W 80
#define TEMP_H 32

GFX_FloatEx lcdTemp = GFX_FloatEx<Arduino_ST7735_Ex>(0.0F, 1, 4, String(static_cast<char>(248)) + "C", 2, tft, background, color, TEMP_X, TEMP_Y, TEMP_W, TEMP_H);

#define CHAR_X 90
#define CHAR_Y 0
#define CHAR_W 38
#define CHAR_H 36

GFX_UChar lcdChar = GFX_UChar<Arduino_ST7735_Ex>(charLCD, 4, tft, background, color, CHAR_X, CHAR_Y);

#define CHRB_X 0
#define CHRB_Y 36
#define CHRB_W 80
#define CHRB_H 16

GFX_Boolean lcdChrB = GFX_Boolean<Arduino_ST7735_Ex>(charLCD % 2, "odd", "even", 2, tft, background, color, CHRB_X, CHRB_Y, CHRB_W, CHRB_H);

#define CHRN_X 80
#define CHRN_Y 36
#define CHRN_W 48
#define CHRN_H 16

GFX_Text lcdChrN = GFX_Text<Arduino_ST7735_Ex>(String(charLCD), 2, tft, background, color, CHRN_X, CHRN_Y, CHRN_W, CHRN_H);

#define CO2_X 0
#define CO2_Y 52
#define CO2_W 128
#define CO2_H 40

GFX_IntegerEx lcdCO2 = GFX_IntegerEx<Arduino_ST7735_Ex>(0, 4, "ppm", 1, tft, background, color, CO2_X, CO2_Y, CO2_W, CO2_H);

#define TIME_Y 95
#define TIME_H 33

#define HOUR_X 0
#define HOUR_W 50

#define SEC_X  (HOUR_X + HOUR_W)
#define SEC_W  28

#define MIN_X  (SEC_X + SEC_W)
#define MIN_W  50

GFX_TextEx lcdH = GFX_TextEx<Arduino_ST7735_Ex>("00", 3, "H", 1, tft, background, color, HOUR_X, TIME_Y, HOUR_W, TIME_H);
GFX_UCharEx lcdS = GFX_UCharEx<Arduino_ST7735_Ex>(':', 3, "S", 1, tft, background, color, SEC_X, TIME_Y, SEC_W, TIME_H);
GFX_TextEx lcdM = GFX_TextEx<Arduino_ST7735_Ex>("00", 3, "M", 1, tft, background, color, MIN_X, TIME_Y, MIN_W, TIME_H);

// -------------------------------------------------------------------------------------------------------------

void setup_TFT()
{
	log_println(F("LCD init..."));

	// TFT init
	tft->begin();
	tft->setTextWrap(false);			// Allow text to run off right edge
	tft->fillScreen(background);

	lcdTemp.setDecimalSize(2);
	lcdTemp.show();

	lcdChar.show();
	lcdChrN.show();
	lcdChrB.show();

	lcdCO2.show();

	lcdH.show();
	lcdS.show();
	lcdM.show();
}

void setup(void)
{
#ifdef DEBUG_OUTPUT
	Serial.begin(115200);
#endif

	setup_TFT();
}

void loop(void)
{
	lcdTemp.updateValue(random(-4000, 4000) / 100.0);

	// ---

	lcdChar.updateValue(--charLCD);
	lcdChrN.updateText(String(charLCD));
	lcdChrB.updateValue(charLCD % 2);

	// ---

	lcdCO2.updateValue(random(15000));

	// ---

	char newTimeString[3] = { 0 };

	sprintf(newTimeString, "%02hu", random(24));
	lcdH.updateText(String(newTimeString));

	sprintf(newTimeString, "%02hu", random(60));
	lcdM.updateText(String(newTimeString));

	lcdS.clear();
	delay(1000);
	lcdS.show();
	delay(1000);

	// ---
}
