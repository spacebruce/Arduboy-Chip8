#pragma once

// For millis
#include <Arduino.h>

// For Arduboy2Core::idle
#include <Arduboy2.h>

class FrameRateLimiter
{
private:
	uint8_t frameDurationTarget = 16;
	uint8_t lastFrameTime = 0;

public:
	void setFrameRate(uint8_t rate)
	{
		this->frameDurationTarget = (1000u / rate);
	}

	void setFrameDuration(uint8_t frameDuration)
	{
		this->frameDurationTarget = frameDuration;
	}

	bool shouldRunNextFrame()
	{
		const uint8_t now = millis();
		const uint8_t frameDelta = (now - this->lastFrameTime);

		if(frameDelta < frameDurationTarget)
		{
			if((frameDelta + 1) < frameDurationTarget)
				Arduboy2Core::idle();

			return false;
		}

		lastFrameTime = now;
		return true;
	}
};