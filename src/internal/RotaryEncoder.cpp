#include "RotaryEncoder.h"

namespace ArduLibs
{

	RotaryEncoder::RotaryEncoder(uint8_t leftPin, uint8_t rightPin, EncoderEventHandler eventHandler) : pinLeft(leftPin), pinRight(rightPin), eventHandler(eventHandler)
	{
	}

	RotaryEncoder::~RotaryEncoder()
	{
	}

	uint8_t RotaryEncoder::getPinLeft() const
	{
		return pinLeft;
	}

	uint8_t RotaryEncoder::getPinRight() const
	{
		return pinRight;
	}

	void RotaryEncoder::setEventHandler(EncoderEventHandler eventHandler)
	{
		this->eventHandler = eventHandler;
	}

	void RotaryEncoder::clearEventHandler()
	{
		this->eventHandler = nullptr;
	}

	bool RotaryEncoder::catchTicks()
	{
		uint8_t curState = getState();

		if (prevState != curState)
		{
			prevState = curState;

			sequence <<= 2;

			sequence |= curState;

			if (eventHandler)
			{
				if (sequence == 0b01001011)
				{
					eventHandler(this, ENCODER_EVENT::RIGHT);
				}
				else if (sequence == 0b10000111)
				{
					eventHandler(this, ENCODER_EVENT::LEFT);
				}
			}
		}

		return curState != 0b11;
	}

}