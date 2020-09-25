#pragma once
#include <stdint.h>
#include <stddef.h>
#include <Arduboy2.h>
#include "config.h"
#include "chip8.h"

enum class DebugScreenView : uint8_t
{
  Decimal8,
  Decimal16,

};

class DebugModule
{
private:
  Chip8* Emulator;
  size_t Scroll = 0x200;
  DebugScreenView ViewMode = DebugScreenView::Decimal8;
public:
  DebugModule(Chip8* Emulator);
  void Tick(Arduboy2* System);
  void Draw(Arduboy2* System);
};
