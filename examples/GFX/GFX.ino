#define DEBUG_OUTPUT

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

#define CO2_X 0
#define CO2_Y 5
#define CO2_W 128

#define TEMP_X 5
#define TEMP_Y 40

#define HUM_X 5
#define HUM_Y 75

#define TIME_Y 110
#define TIME_H 18

#define HOUR_X 20
#define HOUR_W 40

#define SEC_X  (HOUR_X + HOUR_W)
#define SEC_W  8

#define MIN_X  (SEC_X + SEC_W)
#define MIN_W  40

GFX_Integer lcdCO2 = GFX_Integer(0, &tft, 3, background, color, CO2_X, CO2_Y, CO2_W);

GFX_Float lcdTemp = GFX_Float(0.0F, 2, &tft, 4, background, color, TEMP_X, TEMP_Y);

GFX_Float lcdHum = GFX_Float(0.0F, 1, &tft, 4, background, color, HUM_X, HUM_Y);

GFX_String lcdH = GFX_String("00", &tft, 2, background, color, HOUR_X, TIME_Y, HOUR_W, TIME_H);
GFX_String lcdS = GFX_String(":", &tft, 2, background, color, SEC_X, TIME_Y, SEC_W, TIME_H);
GFX_String lcdM = GFX_String("00", &tft, 2, background, color, MIN_X, TIME_Y, MIN_W, TIME_H);

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

	lcdTemp.setDecimalSize(3, false);
	lcdTemp.setExtension("C", 2, false);
	lcdTemp.show();

	lcdHum.setDecimalSize(2, false);
	lcdHum.setExtension("%", 2, false);
	lcdHum.show();

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
	lcdCO2.setValue(random(15000));

	lcdTemp.setValue(random(-4000, 4000) / 100.0);

	lcdHum.setValue(random(9999) / 100.0);

	char newTimeString[2] = { 0 };

	sprintf(newTimeString, "%02hu", random(24));
	lcdH.setValue(String(newTimeString), true);

	sprintf(newTimeString, "%02hu", random(60));
	lcdM.setValue(String(newTimeString), true);

	lcdS.clear();
	delay(500);
	lcdS.show();
	delay(500);

	delay(1000);
}
