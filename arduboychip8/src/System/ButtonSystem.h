#pragma once

#include <Arduboy2.h>

class ButtonSystem
{
private:
	uint8_t previousButtonStates = 0x00;
	uint8_t currentButtonStates = 0x00;

public:
	void updateButtons()
	{
		this->previousButtonStates = this->currentButtonStates;
		this->currentButtonStates = Arduboy2Core::buttonsState();
	}

	bool isPressed(uint8_t buttons)
	{
		return ((Arduboy2Core::buttonsState() & buttons) == buttons);
	}

	bool anyPressed(uint8_t buttons)
	{
		return ((Arduboy2Core::buttonsState() & buttons) != 0);
	}

	bool notPressed(uint8_t buttons)
	{
		return ((Arduboy2Core::buttonsState() & buttons) == 0);
	}

	uint8_t getJustPressed() const
	{
		return (~this->previousButtonStates & this->currentButtonStates);
	}

	uint8_t getJustReleased() const
	{
		return (this->previousButtonStates & ~this->currentButtonStates);
	}

	bool justPressed(uint8_t button)
	{
		return ((this->getJustPressed() & button) != 0);
	}

	bool justReleased(uint8_t button)
	{
		return ((this->getJustReleased() & button) != 0);
	}
};