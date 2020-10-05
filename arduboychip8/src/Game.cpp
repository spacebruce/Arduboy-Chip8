#include "Game.h"

#include "GameList.h"

void Game::setup()
{
  Arduboy2Core::boot();
  Arduboy2Base::flashlight();
  Arduboy2Base::systemButtons();
  Arduboy2Audio::begin();

  this->screen.clear();
  this->frameRateLimiter.setFrameRate(60);

  BeepPin1::begin();
}

void Game::loop()
{
  if (!this->frameRateLimiter.shouldRunNextFrame())
    return;

  this->buttonSystem.updateButtons();
  this->screen.clear();
  this->printer.clear();
  
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

  this->screen.display();
}
