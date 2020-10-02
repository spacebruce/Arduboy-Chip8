#pragma once

#include <Arduboy2.h>

#include "States.h"
#include "GameState.h"
#include "Chip8Emulator.h"
#include "System.h"

class Game
{
private:
	Arduboy2 arduboy;
	Chip8Emulator emulator;

	GameState gameState;

	MenuState menuState;
	EmulatorState emulatorState;

	ButtonSystem buttonSystem;
	FrameRateLimiter frameRateLimiter;
	Screen64x32 screen;

public:
	GameState getGameState() const
	{
		return this->gameState;
	}

	void setGameState(GameState gameState)
	{
		this->gameState = gameState;
	}

	Chip8Emulator & getEmulator()
	{
		return this->emulator;
	}

	ButtonSystem & getButtonSystem()
	{
		return this->buttonSystem;
	}

	Screen64x32 & getScreen()
	{
		return this->screen;
	}

public:
	void setup();
	void loop();
};