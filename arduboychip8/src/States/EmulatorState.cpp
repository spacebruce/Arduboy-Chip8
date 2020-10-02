#include "EmulatorState.h"

#include "../Game.h"
#include "../Chip8Emulator.h"

void EmulatorState::update(Game & game)
{
	Chip8Emulator & emulator = game.getEmulator();

	emulator.UpdateDelayTimer();
	emulator.UpdateSoundTimer();
	emulator.ClearInput();

	this->updateInput(game);

	emulator.Tick(screen, 50);
}

void EmulatorState::render(Game & game)
{
	static_cast<void>(game);
	game.getScreen().fillBufferFromMemory(screen.buffer);
}

void EmulatorState::updateInput(Game & game)
{
	Chip8Emulator & emulator = game.getEmulator();
	ButtonSystem & buttonSystem = game.getButtonSystem();

	switch(game.getInputMode())
	{
		case InputMode::Chord:
		{
			uint8_t Offset = 0;

			if(buttonSystem.isPressed(A_BUTTON))
				Offset += 4;

			if(buttonSystem.isPressed(B_BUTTON))
				Offset += 8;

			if(buttonSystem.isPressed(UP_BUTTON))
				emulator.SendInput(Offset + 0);

			if(buttonSystem.isPressed(RIGHT_BUTTON))
				emulator.SendInput(Offset + 1);

			if(buttonSystem.isPressed(DOWN_BUTTON))
				emulator.SendInput(Offset + 2);

			if(buttonSystem.isPressed(RIGHT_BUTTON))
				emulator.SendInput(Offset + 3);
			}
		break;

		case InputMode::Invaders:
		{
			if(buttonSystem.isPressed(A_BUTTON))
				emulator.SendInput(0x5);

			if(buttonSystem.isPressed(UP_BUTTON))
				emulator.SendInput(0x2);

			if(buttonSystem.isPressed(LEFT_BUTTON))
				emulator.SendInput(0x4);

			if(buttonSystem.isPressed(RIGHT_BUTTON))
				emulator.SendInput(0x6);

			if(buttonSystem.isPressed(DOWN_BUTTON))
				emulator.SendInput(0x8);
		}
		break;

		case InputMode::Tetris:
			if(buttonSystem.isPressed(A_BUTTON))
				emulator.SendInput(0x4);

			if(buttonSystem.isPressed(LEFT_BUTTON))
				emulator.SendInput(0x5);

			if(buttonSystem.isPressed(RIGHT_BUTTON))
				emulator.SendInput(0x6);
		break;
	}
}
