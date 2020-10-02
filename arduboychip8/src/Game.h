#pragma once

#include <Arduboy2.h>

#include "States.h"
#include "GameState.h"
#include "Chip8Emulator.h"
#include "InputMode.h"
#include "System.h"

class Game
{
private:
	Chip8Emulator emulator;

	GameState gameState;
	InputMode inputMode = InputMode::Chord;

	MenuState menuState;
	EmulatorState emulatorState;

	ButtonSystem buttonSystem;
	FrameRateLimiter frameRateLimiter;
	Screen64x32 screen;
	Printer printer = Printer(screen.buffer, Screen64x32::bufferWidth, Screen64x32::bufferHeight);

public:
	GameState getGameState() const
	{
		return this->gameState;
	}

	void setGameState(GameState gameState)
	{
		this->gameState = gameState;
	}

	InputMode getInputMode() const
	{
		return this->inputMode;
	}

	void setInputMode(InputMode inputMode)
	{
		this->inputMode = inputMode;
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
