#pragma once
#include "../Menu.h"

class Game;

class MenuState
{
private:
	Menu menu = Menu();
public:
	void update(Game & game);
	void render(Game & game);
};
