#include "EmulatorState.h"

#include "../Game.h"
#include "../Chip8Emulator.h"

void EmulatorState::update(Game & game)
{
	Chip8Emulator & emulator = game.getEmulator();

	emulator.UpdateDelayTimer();
	emulator.UpdateSoundTimer();
	emulator.ClearInput();
	emulator.Tick(game.getScreen(), 50);
}

void EmulatorState::render(Game & game)
{
	static_cast<void>(game);
}