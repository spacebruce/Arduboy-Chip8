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
#if SMALL_MEMORY
    System->print(F("SM"));
#endif
  switch(this->ViewMode)
  {
    case DebugScreenView::Decimal8:
      System->println(F("view : 8 bit"));
      for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
      {
        System->print(i);
        switch(Emulator->GetMemoryPartition(i))
        {
          case MemoryPartition::RAM:
          System->print(F(" | RAM | "));
          break;
          case MemoryPartition::ROM:
          System->print(F(" | ROM | "));
          break;
          case MemoryPartition::Static:
          System->print(F(" | SYS | "));
          break;
        }
        System->println(Emulator->ReadMemory(i));
      }
    break;
    case DebugScreenView::Decimal16:
      System->println(F("view : 16 bit"));
      size_t pointer = floor(this->Scroll / 2) * 2;
      for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
      {
        int16_t value = (Emulator->ReadMemory(pointer) << 8) + (Emulator->ReadMemory(pointer + 1));
        System->print(pointer);
        switch(Emulator->GetMemoryPartition(pointer))
        {
          case MemoryPartition::RAM:
          System->print(F(" | RAM | "));
          break;
          case MemoryPartition::ROM:
          System->print(F(" | ROM | "));
          break;
          case MemoryPartition::Static:
          System->print(F(" | SYS | "));
          break;
        }
        System->println(value);
        pointer += 2;
      }
    break;
  }
}
