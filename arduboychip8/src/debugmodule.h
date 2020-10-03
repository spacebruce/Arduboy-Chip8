#pragma once
#include <stdint.h>
#include <stddef.h>
#include <Arduboy2.h>
#include "config.h"
#include "Chip8Emulator.h"

enum class DebugScreenView : uint8_t
{
  Memory8,
  Memory16,
  Processor,
  Stack,
  Input,
  Sizeof,
};

constexpr DebugScreenView firstDebugScreenView = DebugScreenView::Memory8;
constexpr DebugScreenView lastDebugScreenView = DebugScreenView::Sizeof;

class DebugModule
{
private:
  Chip8Emulator * Emulator;
  size_t Scroll = 0x200;
  uint16_t CurrentOpcode = 0x0000;
  DebugScreenView ViewMode = DebugScreenView::Memory8;
  uint8_t Input[16];
  uint8_t InputSelected = 0;

public:
  DebugModule(Chip8Emulator & Emulator);
  void Tick(Arduboy2 & System);
  void Draw(Arduboy2 & System);

private:
  void ClearDebugArea();
  void DrawMemory8View(Arduboy2 & System);
  void DrawMemory16View(Arduboy2 & System);
  void DrawProcessorView(Arduboy2 & System);
  void DrawStackView(Arduboy2 & System);
  void DrawInputView(Arduboy2 & System);
  void DrawError(Arduboy2 & System);
};
