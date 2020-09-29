#pragma once
#include "Chip8Emulator.h"

enum class Chip8InputMode : uint8_t
{
  Chord, Invaders, Tetris
};

/*  Modes
  Chord:
    U/R/D/L : 0,1,2,3
    A + U,R,D,L : 4,5,6,7
    B + U,R,D,L : 8,9,10,11
    A + B + U,R,D,L : 12,13,14,15
  Invaders:
    U,L,R,D,A : 2,4,6,8,5
  Tetris:
    A,L,R : 4,5,6
*/

class Chip8Input
{
private:
  Chip8Emulator* Emulator;
  Chip8InputMode InputMode = Chip8InputMode::Chord;
public:
  Chip8Input(Chip8Emulator& Emulator);
  Chip8Input(Chip8Emulator& Emulator, Chip8InputMode InputMode);
  void ChangeMode(const Chip8InputMode InputMode);
  void Tick(Arduboy2& System);
};
