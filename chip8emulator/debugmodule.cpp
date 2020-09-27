#include "debugmodule.h"

DebugModule::DebugModule(Chip8 & Emulator)
{
  this->Emulator = &Emulator;
}

void DebugModule::Tick(Arduboy2 & System)
{
//Mode
  if(System.justPressed(LEFT_BUTTON))
  {
    this->ViewMode = static_cast<DebugScreenView>((static_cast<uint8_t>(this->ViewMode) - 1) % static_cast<uint8_t>(DebugScreenView::Sizeof));
    if(this->ViewMode == DebugScreenView::End)
      this->ViewMode = static_cast<DebugScreenView>(static_cast<uint8_t>(DebugScreenView::Sizeof) - 1);
  }
  if(System.justPressed(RIGHT_BUTTON))
  {
    this->ViewMode = static_cast<DebugScreenView>((static_cast<uint8_t>(this->ViewMode) + 1) % static_cast<uint8_t>(DebugScreenView::Sizeof));
  }

//Scroll
  if(System.pressed(UP_BUTTON))
  {
    if(this->Scroll > 0)
      --(this->Scroll);
  }
  if(System.pressed(DOWN_BUTTON))
  {
    this->Scroll = (this->Scroll + 1);
  }
}

void DebugModule::Draw(Arduboy2 & System)
{
  //Clear out screen behind debug gui
  uint8_t height  = 32;
  for(uint8_t i = 0; i < 128; ++i)
  {
    if(i > 64)  height = 0;
    System.drawFastVLine(i,height,64 - height,BLACK);
  }

  ///Memory viewer
  uint8_t line = 1;

  if(this->ViewMode == DebugScreenView::Decimal8)
  {
    System.setCursor(65, 0);
    System.println(F("8 bit"));
    for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
    {
      System.setCursor(70, line * 8); ++line;
      System.print(i);
      System.print("|");
      System.println(Emulator->ReadMemory(i), HEX);
    }
  }
  if(this->ViewMode == DebugScreenView::Decimal16)
  {
    System.setCursor(65, 0);
    System.println(F("16 bit"));
    size_t pointer = floor(this->Scroll / 2) * 2;
    for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
    {
      System.setCursor(70, line * 8); ++line;
      int16_t value = (Emulator->ReadMemory(pointer) << 8) + (Emulator->ReadMemory(pointer + 1));
      System.print(pointer);
      System.print("|");
      System.println(value, HEX);
      pointer += 2;
    }
  }
  if(this->ViewMode == DebugScreenView::Processor)
  {
    System.setCursor(0,32);
    uint8_t High = Emulator->ReadMemory(Emulator->ProgramCounter - 2);
    uint8_t Low = Emulator->ReadMemory(Emulator->ProgramCounter - 1);
    uint16_t Opcode = (High << 8) | Low;
    System.print(High, HEX);
    System.print(",");
    System.print(Low, HEX);
    System.print("x");
    System.print(Opcode, HEX);
    System.setCursor(65,0);
    System.print("DEBUG CPU");
    System.setCursor(65,8);
    System.print("PC:");  System.print(Emulator->ProgramCounter);
    System.setCursor(65,16);
    System.print("I :");  System.print(Emulator->Index);
    System.setCursor(80,24);
    System.print("TD:");  System.print(Emulator->TimerDelay);
    System.setCursor(80,32);
    System.print("TS:");  System.print(Emulator->TimerSound);
    System.setCursor(0,40);
    for(uint8_t i = 0; i < 8; ++i)
    {
      System.setCursor(i*16, 40);
      System.print(Emulator->Register[i], HEX);
      System.setCursor(i*16, 48);
      System.print(Emulator->Register[i+8], HEX);
    }
  }

  if(Emulator->Mode != CPUMode::Running)
  {
    System.setCursor(0,56);
    if(Emulator->Mode == CPUMode::Error)
    {
      System.print(F("ERR - "));
      switch(Emulator->Error)
      {
        case CPUError::MemoryWrite:
          System.print(F("MEMWR : "));
          System.print(Emulator->ErrorData);
        break;
        case CPUError::MemoryRead:
          System.print(F("MEMRE : "));
          System.print(Emulator->ErrorData);
        break;
        case CPUError::UnknownOpcode:
          System.print(F("UNKNW : "));
          System.print(Emulator->ErrorData, HEX);
        break;
      }
    }
  }
  if(Emulator->Mode == CPUMode::Stopped)
  {
    System.print(F("CPU HALT"));
  }
  if(Emulator->Mode == CPUMode::Startup)
  {
    System.print(F("CPU STARTUP"));
  }
}
