#include "Chip8Input.h"

Chip8Input::Chip8Input(Chip8Emulator& Emulator)
{
  this->Emulator = &Emulator;
}
Chip8Input::Chip8Input(Chip8Emulator& Emulator, Chip8InputMode InputMode)
{
  this->Emulator = &Emulator;
  this->InputMode = InputMode;
}

void Chip8Input::ChangeMode(const Chip8InputMode InputMode)
{
  this->InputMode = InputMode;
}

void Chip8Input::Tick(Arduboy2& System)
{
  switch(this->InputMode)
  {
    case Chip8InputMode::Chord:
    {
      uint8_t Offset = 0;
      if(System.pressed(A_BUTTON))
        Offset += 4;
      if(System.pressed(B_BUTTON))
        Offset += 8;
      if(System.pressed(UP_BUTTON))
        Emulator->SendInput(Offset + 0);
      if(System.pressed(RIGHT_BUTTON))
        Emulator->SendInput(Offset + 1);
      if(System.pressed(DOWN_BUTTON))
        Emulator->SendInput(Offset + 2);
      if(System.pressed(RIGHT_BUTTON))
        Emulator->SendInput(Offset + 3);
      }
    break;
    case Chip8InputMode::Invaders:
    {
      if(System.pressed(A_BUTTON))
        Emulator->SendInput(0x5);
      if(System.pressed(UP_BUTTON))
        Emulator->SendInput(0x2);
      if(System.pressed(LEFT_BUTTON))
        Emulator->SendInput(0x4);
      if(System.pressed(RIGHT_BUTTON))
        Emulator->SendInput(0x6);
      if(System.pressed(DOWN_BUTTON))
        Emulator->SendInput(0x8);
    }
    break;
  }
}
