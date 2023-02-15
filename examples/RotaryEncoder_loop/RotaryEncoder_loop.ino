#include "ArduinoRotaryEncoder.h"

#define ENC1_PIN D1 // encoder S1 pin
#define ENC2_PIN D2	// encoder S2 pin

void handleEncoderEvent(const RotaryEncoder*, ENCODER_EVENT);

ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN, handleEncoderEvent);

void handleEncoderEvent(const RotaryEncoder* enc, ENCODER_EVENT eventType)
{
	switch (eventType)
	{
	case ENCODER_EVENT::LEFT:
		Serial.println(F("LEFT"));
		break;
	case ENCODER_EVENT::RIGHT:
		Serial.println(F("RIGHT"));
		break;
	default:
		break;
	}
}

void setup()
{
	Serial.begin(115200);
	Serial.println(F("RotaryEncoder"));

	encoder.initPins();
}

void loop()
{
	if (encoder.catchTicks())
	{
		Serial.println(F("rotating"));
	}
}
