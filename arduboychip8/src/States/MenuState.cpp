#include "MenuState.h"

#include <Arduboy2.h>

#include "../Game.h"

void MenuState::update(Game & game)
{
	auto & buttonSystem = game.getButtonSystem();

	if(buttonSystem.pressed(A_BUTTON))
		game.setGameState(GameState::Emulator);
}

void MenuState::render(Game & game)
{
	static_cast<void>(game);
}