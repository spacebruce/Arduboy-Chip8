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
  Processor,
  Stack,
  Sizeof,
  End = 255,
};

class DebugModule
{
private:
  Chip8 * Emulator;
  size_t Scroll = 0x200;
  uint16_t CurrentOpcode = 0x0000;
  DebugScreenView ViewMode = DebugScreenView::Decimal8;
public:
  DebugModule(Chip8 & Emulator);
  void PreTick();
  void Tick(Arduboy2 & System);
  void Draw(Arduboy2 & System);
};
