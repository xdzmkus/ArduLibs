#ifndef _SERIAL_DEBUG_HPP_
#define _SERIAL_DEBUG_HPP_

#define SERIAL_DEBUG

#include "Arduino.h"

namespace ArduLibs
{

	enum class LOG_LEVEL
	{
		TRACE,
		INFO,
		WARN,
		ERROR,
		USER,
		NONE,
		DEBUG
	};

	class SerialDebug
	{
	public:

		static void begin(unsigned long baud = 115200)
		{
#ifdef SERIAL_DEBUG
			Serial.begin(baud);
#endif
		};

		static void print(LOG_LEVEL level, String msg)
		{
#ifdef SERIAL_DEBUG
			Serial.print(getLevelString(level));
			Serial.print(msg);
#endif
		};

		static void println(LOG_LEVEL level, String msg)
		{
#ifdef SERIAL_DEBUG
			Serial.print(getLevelString(level));
			Serial.println(msg);
#endif
		};

	private:

		static String getLevelString(LOG_LEVEL level)
		{
			switch (level)
			{
			case LOG_LEVEL::INFO:	return "INFO: ";
			case LOG_LEVEL::WARN:	return "WARNING: ";
			case LOG_LEVEL::ERROR:	return "ERROR: ";
			case LOG_LEVEL::TRACE:	return "TRACE: ";
			case LOG_LEVEL::DEBUG:	return "DEBUG: ";
			case LOG_LEVEL::USER:	return "USER: ";
			case LOG_LEVEL::NONE:
			default:				return "";
			}
		};
	};


}
#endif
