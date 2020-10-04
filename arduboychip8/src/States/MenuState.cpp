#include "MenuState.h"

#include <Arduboy2.h>

#include "../Game.h"

void MenuState::update(Game & game)
{
	auto & buttonSystem = game.getButtonSystem();

	if(buttonSystem.isPressed(A_BUTTON))
		game.setGameState(GameState::Emulator);
}

void MenuState::render(Game & game)
{
	auto & printer = game.getPrinter();
	printer.setPosition(0,0);
	printer.print("0123456789!?");
	printer.setPosition(0,8);
	printer.print("ABCDEFGHIJK");
	printer.setPosition(0,16);
	printer.print("LMNOPQRSTUV");
	printer.setPosition(0,24);
	printer.print("WXYZ");
}
