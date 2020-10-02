#include "Game.h"

#include "testrom.h"

void Game::setup()
{
  this->arduboy.begin();
  this->arduboy.clear();
  this->frameRateLimiter.setFrameRate(60);

  BeepPin1::begin();

  this->emulator.Load(TestRom);
}

void Game::loop()
{
  if (!this->frameRateLimiter.shouldRunNextFrame())
    return;

  this->buttonSystem.updateButtons();

  switch(this->gameState)
  {
    case GameState::Menu:
      this->menuState.update(*this);
      this->menuState.render(*this);
      break;

    case GameState::Emulator:
      this->emulatorState.update(*this);
      this->emulatorState.render(*this);
      break;
  }

  this->arduboy.display();
}