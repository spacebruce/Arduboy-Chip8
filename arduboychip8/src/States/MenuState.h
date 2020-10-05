#pragma once
#include "../MainMenu.h"

class Game;

class MenuState
{
private:
	MainMenu menu;
public:
	void update(Game & game);
	void render(Game & game);
};
