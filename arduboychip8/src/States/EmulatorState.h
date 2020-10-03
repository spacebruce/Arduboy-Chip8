#pragma once
#include "../System/Screen64x32.h"

class Game;

class EmulatorState
{
public:
	void update(Game & game);
	void render(Game & game);
private:
	void updateInput(Game & game);
};
