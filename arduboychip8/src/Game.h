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
	Screen64x32 emulatorScreen;
	Printer printer = Printer(screen);

public:
	GameState getGameState() const
	{
		return this->gameState;
	}

	void emulatorStart(const uint8_t* Rom, const size_t Length)
	{
		this->emulator.Load(Rom, Length);
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
	Screen64x32 & getEmulatorScreen()
	{
		return this->emulatorScreen;
	}

	Printer & getPrinter()
	{
		return this->printer;
	}

public:
	void setup();
	void loop();
};
