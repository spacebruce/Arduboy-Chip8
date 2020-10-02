#pragma once
#include "../System/Screen64x32.h"

class Game;

class EmulatorState
{
public:
	void update(Game & game);
	void render(Game & game);
	Screen64x32 screen;
private:
	void updateInput(Game & game);
};
