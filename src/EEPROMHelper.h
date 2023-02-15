#ifndef _EEPROM_HELPER_H_
#define _EEPROM_HELPER_H_

#include <EEPROM.h>

#include "SerialDebug.h"

class EEPROMHelper
{
public:

	static void begin(size_t size)
	{
		EEPROM.begin(size);
	}

	static String readEEPROM(int offset, unsigned int len)
	{
		String res = "";
		for (unsigned int i = 0; i < len; ++i)
		{
			res += char(EEPROM.read(i + offset));
		}
		SerialDebug::log(LOG_LEVEL::DEBUG, String(F("readEEPROM(): ")) + res);
		return res;
	}

	static void writeEEPROM(int offset, unsigned int len, String value)
	{
		SerialDebug::log(LOG_LEVEL::DEBUG, String(F("writeEEPROM(): ")) + value);
		for (unsigned int i = 0; i < len; ++i)
		{
			EEPROM.write(i + offset, i < value.length() ? value[i] : 0);
		}
	}

	static bool commit()
	{
		return EEPROM.commit();
	}

};

#endif
