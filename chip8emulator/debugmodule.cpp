#include "debugmodule.h"

DebugModule::DebugModule(Chip8* Emulator)
{
  this->Emulator = Emulator;
}

void DebugModule::Tick(Arduboy2* System)
{
//Mode
  if(System->justPressed(LEFT_BUTTON))
  {
    this->ViewMode = static_cast<DebugScreenView>(static_cast<uint8_t>(this->ViewMode) - 1);
  }
  if(System->justPressed(RIGHT_BUTTON))
  {
    this->ViewMode = static_cast<DebugScreenView>(static_cast<uint8_t>(this->ViewMode) + 1);
  }
//Scroll
  if(System->pressed(UP_BUTTON))
  {
    if(this->Scroll > 0)
      --(this->Scroll);
  }
  if(System->pressed(DOWN_BUTTON))
  {
    this->Scroll = (this->Scroll + 1);
  }
}

void DebugModule::Draw(Arduboy2* System)
{
  System->setCursor(64, 0);
#if SMALL_MEMORY
    System->print(F("mem "));
#endif
  uint8_t line = 1;
  switch(this->ViewMode)
  {
    case DebugScreenView::Decimal8:
      System->println(F("8 bit"));
      for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
      {
        System->setCursor(64, line * 8); ++line;
        System->print(i);
        System->print("|");
        System->println(Emulator->ReadMemory(i));
      }
    break;
    case DebugScreenView::Decimal16:
      System->println(F("16 bit"));
      size_t pointer = floor(this->Scroll / 2) * 2;
      for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
      {
        System->setCursor(64, line * 8); ++line;
        int16_t value = (Emulator->ReadMemory(pointer) << 8) + (Emulator->ReadMemory(pointer + 1));
        System->print(pointer);
        System->print("|");
        System->println(value);
        pointer += 2;
      }
    break;
  }
}
