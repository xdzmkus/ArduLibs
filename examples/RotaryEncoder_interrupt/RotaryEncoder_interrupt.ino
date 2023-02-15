#include "ArduinoRotaryEncoder.h"
#include "EventsQueue.hpp"

#define ENC1_PIN D1 // encoder S1 pin
#define ENC2_PIN D2	// encoder S2 pin

ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

EventsQueue<ENCODER_EVENT, 32> queue;

#if defined(ESP8266)
IRAM_ATTR
#endif
void catchEncoderTicks()
{
	encoder.catchTicks();
}

void handleEncoderEvent(const RotaryEncoder* enc, ENCODER_EVENT eventType)
{
	queue.push(eventType);
}

void setup()
{
	Serial.begin(115200);
	Serial.println(F("RotaryEncoder"));

	encoder.initPins();

	encoder.setEventHandler(handleEncoderEvent);

	attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
}

void loop()
{
	bool processEncEvent;
	ENCODER_EVENT encEvent;

	do
	{
		noInterrupts();

		processEncEvent = queue.length();

		if (processEncEvent)
		{
			encEvent = queue.pop();
		}

		interrupts();

		if (processEncEvent)
		{
			switch (encEvent)
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

	} while (processEncEvent);

	delay(2000);
}
