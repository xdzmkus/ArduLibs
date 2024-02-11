#ifndef _ROTARYENCODER_PROPERTIES_H_
#define _ROTARYENCODER_PROPERTIES_H_

namespace ArduLibs
{

	class RotaryEncoder;

	/**
	* @param LEFT - encoder left tick
	* @param RIGHT - encoder right tick
	*/
	enum class ENCODER_EVENT { LEFT, RIGHT };

	/**
	* The event handler procedure.
	*
	* @param encoder - pointer to the Encoder that generated the event
	* @param eventType - the event type which trigger the call
	*/
	typedef void (*EncoderEventHandler)(const RotaryEncoder* encoder, ENCODER_EVENT eventType);

}

#endif
