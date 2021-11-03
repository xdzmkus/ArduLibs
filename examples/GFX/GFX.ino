#define DEBUG_OUTPUT
#define DEBUG_DRAW

#ifndef DEBUG_OUTPUT
#define log_print(msg)
#define log_println(msg)
#else
#define log_print(msg) Serial.print(msg)
#define log_println(msg) Serial.println(msg)
#endif

#include "GFX_Text.h"
#include "GFX_TextEx.h"
#include "GFX_Float.h"
#include "GFX_FloatEx.h"
#include "GFX_Integer.h"
#include "GFX_IntegerEx.h"
#include "GFX_UChar.h"
#include "GFX_UCharEx.h"
#include "GFX_Boolean.h"

// *** TFT-1.4 *** //
#include <Adafruit_ST7735.h>

#define TFT_RST   -1
#define TFT_CS    D4
#define TFT_DC    D3

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable. For Wemos D1 mini:
// MOSI = pin D7 and SCLK = pin D5.
// This is the fastest mode of operation and is required if
// using the breakout board's microSD card.
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
//#define TFT_MOSI 11  // Data out
//#define TFT_SCLK 13  // Clock out
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

unsigned int background = ST7735_WHITE;
unsigned int color = ST7735_BLACK;

unsigned char charLCD = 0;

#define TEMP_X 0
#define TEMP_Y 4
#define TEMP_W 80
#define TEMP_H 32

GFX_FloatEx lcdTemp = GFX_FloatEx(0.0F, 1, 4, String(static_cast<char>(248)) +"C", 2, &tft, background, color, TEMP_X, TEMP_Y, TEMP_W, TEMP_H);

#define CHAR_X 90
#define CHAR_Y 0
#define CHAR_W 38
#define CHAR_H 36

GFX_UChar lcdChar = GFX_UChar(charLCD, 4, &tft, background, color, CHAR_X, CHAR_Y);

#define CHRB_X 0
#define CHRB_Y 36
#define CHRB_W 80
#define CHRB_H 16

GFX_Boolean lcdChrB = GFX_Boolean(charLCD % 2, "odd", "even", 2, &tft, background, color, CHRB_X, CHRB_Y, CHRB_W, CHRB_H);

#define CHRN_X 80
#define CHRN_Y 36
#define CHRN_W 48
#define CHRN_H 16

GFX_Text lcdChrN = GFX_Text(String(charLCD), 2, &tft, background, color, CHRN_X, CHRN_Y, CHRN_W, CHRN_H);

#define CO2_X 0
#define CO2_Y 52
#define CO2_W 128
#define CO2_H 40

GFX_IntegerEx lcdCO2 = GFX_IntegerEx(0, 4, "ppm", 1, &tft, background, color, CO2_X, CO2_Y, CO2_W, CO2_H);

#define TIME_Y 95
#define TIME_H 33

#define HOUR_X 0
#define HOUR_W 50

#define SEC_X  (HOUR_X + HOUR_W)
#define SEC_W  28

#define MIN_X  (SEC_X + SEC_W)
#define MIN_W  50

GFX_TextEx lcdH = GFX_TextEx("00", 3, "H", 1, &tft, background, color, HOUR_X, TIME_Y, HOUR_W, TIME_H);
GFX_UCharEx lcdS = GFX_UCharEx(':', 3, "S", 1, &tft, background, color, SEC_X, TIME_Y, SEC_W, TIME_H);
GFX_TextEx lcdM = GFX_TextEx("00", 3, "M", 1, &tft, background, color, MIN_X, TIME_Y, MIN_W, TIME_H);

// -------------------------------------------------------------------------------------------------------------

void setup_TFT()
{
	log_println(F("LCD init..."));

	// TFT init
	tft.initR(INITR_144GREENTAB);	// Init ST7735R chip, green tab
	tft.cp437();
	tft.setRotation(0);
	tft.setTextWrap(false);			// Allow text to run off right edge
	tft.fillScreen(background);

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
