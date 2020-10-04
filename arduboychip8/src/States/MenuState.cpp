#include "MenuState.h"

#include <Arduboy2.h>

#include "../Game.h"

void MenuState::update(Game & game)
{
	auto & buttonSystem = game.getButtonSystem();
	menu.update(buttonSystem);
	if(menu.loadGame())
	{
		game.setGameState(GameState::Emulator);
	}
}

void MenuState::render(Game & game)
{
	auto & printer = game.getPrinter();
	printer.clear();
	printer.print(F("Chip8 thingy"));
	menu.render(game.getScreen(), printer);
}
