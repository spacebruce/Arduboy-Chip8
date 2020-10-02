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
	static_cast<void>(game);

	auto & printer = game.getPrinter();
	printer.setPosition(0,0);
	printer.print("Woah!");
}
