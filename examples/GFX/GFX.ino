#define DEBUG_OUTPUT
//#define DEBUG_DRAW

#ifndef DEBUG_OUTPUT
#define log_print(msg)
#define log_println(msg)
#else
#define log_print(msg) Serial.print(msg)
#define log_println(msg) Serial.println(msg)
#endif

#include "GFX_String.h"
#include "GFX_Float.h"
#include "GFX_Integer.h"
#include "GFX_UChar.h"

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

GFX_Float lcdTemp = GFX_Float(0.0F, 1, &tft, 3, background, color, TEMP_X, TEMP_Y, TEMP_W, TEMP_H);

#define CHAR_X 80
#define CHAR_Y 0
#define CHAR_W 48
#define CHAR_H 36

GFX_UChar lcdChar = GFX_UChar(charLCD, &tft, 3, background, color, CHAR_X, CHAR_Y, CHAR_W, CHAR_H);

#define CHRN_X 80
#define CHRN_Y 36
#define CHRN_W 48
#define CHRN_H 16

GFX_String lcdChrN = GFX_String(String(charLCD), &tft, 2, background, color, CHRN_X, CHRN_Y, CHRN_W, CHRN_H);

#define CO2_X 0
#define CO2_Y 52
#define CO2_W 128
#define CO2_H 40

GFX_Integer lcdCO2 = GFX_Integer(0, &tft, 4, background, color, CO2_X, CO2_Y, CO2_W, CO2_H);

#define TIME_Y 95
#define TIME_H 33

#define HOUR_X 0
#define HOUR_W 50

#define SEC_X  (HOUR_X + HOUR_W)
#define SEC_W  28

#define MIN_X  (SEC_X + SEC_W)
#define MIN_W  50

GFX_String lcdH = GFX_String("00", &tft, 3, background, color, HOUR_X, TIME_Y, HOUR_W, TIME_H);
GFX_String lcdS = GFX_String(":", &tft, 3, background, color, SEC_X, TIME_Y, SEC_W, TIME_H);
GFX_String lcdM = GFX_String("00", &tft, 3, background, color, MIN_X, TIME_Y, MIN_W, TIME_H);

// -------------------------------------------------------------------------------------------------------------

void setup_TFT()
{
	log_println(F("LCD init..."));

	// TFT init
	tft.initR(INITR_144GREENTAB);	// Init ST7735R chip, green tab
	tft.cp437(true);
	tft.setRotation(0);
	tft.setTextWrap(false);			// Allow text to run off right edge
	tft.fillScreen(background);

	lcdCO2.setExtension("PPM", 1, false);
	lcdCO2.show();

	lcdTemp.setDecimalSize(2, false);
	lcdTemp.setExtension("C", 2, false);
	lcdTemp.show();

	lcdH.show();
	lcdS.show();
	lcdM.show();

	lcdChar.show();
	lcdChrN.show();
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
	lcdTemp.setValue(random(-4000, 4000) / 100.0);

	// ---

	lcdCO2.setValue(random(15000));

	// ---

	char newTimeString[3] = { 0 };

	sprintf(newTimeString, "%02hu", random(24));
	lcdH.setValue(String(newTimeString), true);

	sprintf(newTimeString, "%02hu", random(60));
	lcdM.setValue(String(newTimeString), true);

	lcdS.clear();
	delay(500);
	lcdS.show();
	delay(500);

	// ---

	lcdChar.setValue(--charLCD);
	lcdChrN.setValue(String(charLCD));

	delay(1000);
}
