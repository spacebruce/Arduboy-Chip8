#pragma once

class Game;

class EmulatorState
{
public:
	void update(Game & game);
	void render(Game & game);
};