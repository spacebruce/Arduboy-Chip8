#pragma once
#include <Arduino.h>
#include <avr/pgmspace.h>

// This tricks the arduboy into thinking a progmem string has been created with the `F` macro
constexpr inline const __FlashStringHelper * AsFlashStringHelper(const char * flashString)
{
	return reinterpret_cast<const __FlashStringHelper*>(flashString);
}
