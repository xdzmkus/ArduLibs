#ifndef _ROTARYENCODER_H_
#define _ROTARYENCODER_H_

#include "RotaryEncoderProperties.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <stdint.h>
#endif

namespace ArduLibs
{

    class RotaryEncoder
    {
    public:

        RotaryEncoder(uint8_t leftPin, uint8_t rightPin, EncoderEventHandler eventHandler = nullptr);

        virtual ~RotaryEncoder();

        /**
        * Get the encoder left pin number.
        *
        * @return pin number
        */
        uint8_t getPinLeft() const;

        /**
        * Get the encoder right pin number.
        *
        * @return pin number
        */
        uint8_t getPinRight() const;

        /**
        * Install the EncoderEventHandler function pointer
        *
        * @param eventHandler - the event handler function
        */
        void setEventHandler(EncoderEventHandler eventHandler);

        /**
        * Remove the EncoderEventHandler function pointer
        *
        */
        void clearEventHandler();

        /**
        * Check current state of encoder and process events.
        * This method should be called from the code often.
        * If function is called less often, the ticks may be missed.
        *
        * @return false if encoder is idle (not rotating)
        */
        bool catchTicks();

    protected:

        virtual uint8_t getState() = 0;

        // Encoder left pin number connected to 
        const uint8_t pinLeft;

        // Encoder right pin number connected to 
        const uint8_t pinRight;

        // The event handler function for encoder
        EncoderEventHandler eventHandler = nullptr;

    private:

        uint8_t prevState = 0b00000011;
        uint8_t sequence = 0b00000000;

        // Disable copy-constructor and assignment operator
        RotaryEncoder(const RotaryEncoder&) = delete;
        RotaryEncoder& operator=(const RotaryEncoder&) = delete;
    };

}

#endif