#include "debugmodule.h"

DebugModule::DebugModule(Chip8 & Emulator)
{
  this->Emulator = &Emulator;
}

void DebugModule::PreTick()
{
  size_t Address = Emulator->ProgramCounter;
  CurrentOpcode = (Emulator->ReadMemory(Address) << 8) + Emulator->ReadMemory(Address + 1);
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
//Scroll memory viewer
  if(this->ViewMode == DebugScreenView::Memory8 || this->ViewMode == DebugScreenView::Memory16)
  {
    if(System.pressed(UP_BUTTON))
    {
      if(this->Scroll > 0x200)
        --this->Scroll;
    }
    if(System.pressed(DOWN_BUTTON))
    {
      this->Scroll = (this->Scroll + 1);
    }
  }
//Controller
  if(this->ViewMode == DebugScreenView::Input)
  {
    if(System.justPressed(UP_BUTTON))
    {
      --this->InputSelected;
      if(this->InputSelected == 0xFF)
        this->InputSelected = 0x0F;
    }
    if(System.justPressed(DOWN_BUTTON))
    {
      this->InputSelected = (this->InputSelected + 1) % 0x10;
    }
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

  if(this->ViewMode == DebugScreenView::Memory8)
  {
    System.setCursor(65, 0);
    System.println(F("8 bit"));
    for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
    {
      System.setCursor(70, line * 8); ++line;
      System.print(i);
      System.print(F("|"));
      System.println(Emulator->ReadMemory(i), HEX);
    }
  }
  if(this->ViewMode == DebugScreenView::Memory16)
  {
    System.setCursor(65, 0);
    System.println(F("16 bit"));
    size_t pointer = floor(this->Scroll / 2) * 2;
    for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
    {
      System.setCursor(70, line * 8); ++line;
      int16_t value = (Emulator->ReadMemory(pointer) << 8) + (Emulator->ReadMemory(pointer + 1));
      System.print(pointer);
      System.print(F("|"));
      System.println(value, HEX);
      pointer += 2;
    }
  }
  if(this->ViewMode == DebugScreenView::Processor)
  {
    System.setCursor(0,32);
    uint8_t High = (CurrentOpcode >> 8);
    uint8_t Low = (CurrentOpcode & 0x00FF);
    uint16_t Opcode = (High << 8) | Low;
    System.print(High, HEX);
    System.print(",");
    System.print(Low, HEX);
    System.print("x");
    System.print(Opcode, HEX);
    System.setCursor(65,0);
    System.print(F("DEBUG CPU"));
    System.setCursor(65,8);
    System.print(F("PC:"));  System.print(Emulator->ProgramCounter);
    System.setCursor(65,16);
    System.print(F("I :"));  System.print(Emulator->Index);
    System.setCursor(80,24);
    System.print(F("TD:"));  System.print(Emulator->TimerDelay);
    System.setCursor(80,32);
    System.print(F("TS:"));  System.print(Emulator->TimerSound);
    System.setCursor(0,40);
    for(uint8_t i = 0; i < 8; ++i)
    {
      System.setCursor(i*16, 40);
      System.print(Emulator->Register[i], HEX);
      System.setCursor(i*16, 48);
      System.print(Emulator->Register[i+8], HEX);
    }
  }
  if(this->ViewMode == DebugScreenView::Stack)
  {
    System.setCursor(65,0);
    System.print(F("STACK VIEW"));
    System.setCursor(65,8);
    System.print(F("SP"));
    System.print(Emulator->StackPointer);

    for(uint8_t i = 0; i < Chip8::StackSize / 2; ++i)
    {
      const uint8_t High = ((Emulator->Stack[i] >> 8) & 0xFF);
      const uint8_t Low = ((Emulator->Stack[i] >> 0) & 0xFF);

      const int16_t X = (i * 16);

      System.setCursor(X, 40);
      System.print(High, HEX);

      System.setCursor(X, 48);
      System.print(Low, HEX);
    }
  }
  if(this->ViewMode == DebugScreenView::Input)
  {
    for(uint8_t i = 0; i < 8; ++i)
    {
      const uint8_t High = i;
      const uint8_t Low = i + 8;
      const int16_t X = (i * 16) + 8;

      System.setCursor(X, 40);
      System.print(High, HEX);
      System.setCursor(X, 48);
      System.print(Low, HEX);
    }
    System.setCursor((this->InputSelected % 8) * 16, 40 + (this->InputSelected >= 8) * 8);
    System.print(F(">"));
  }

  if(Emulator->Mode != CPUMode::Running)
  {
    System.setCursor(0,56);
    if(Emulator->Mode == CPUMode::Error)
    {
      System.print(F("ERR - "));
      switch(Emulator->ErrorType)
      {
        case CPUError::ExternalWrite:
          System.print(F("EXTWR : "));
          System.print(Emulator->ErrorData);
        break;
        case CPUError::ExternalRead:
          System.print(F("EXTRE : "));
          System.print(Emulator->ErrorData);
        break;
        case CPUError::UnknownOpcode:
          System.print(F("UNKNW : "));
          System.print(Emulator->ErrorData, HEX);
        break;
        case CPUError::StackOverflow:
          System.print(F("OVER"));
        break;
        case CPUError::StackUnderflow:
          System.print(F("UNDER"));
        break;
          // Pepsi cola
        case CPUError::AbsentWrite:
          System.print(F("ABSWR : "));
          System.print(Emulator->ErrorData);
        break;
        case CPUError::AbsentRead:
          System.print(F("ABSRE : "));
          System.print(Emulator->ErrorData);
        break;
        case CPUError::SystemWrite:
          System.print(F("SYSWR : "));
          System.print(Emulator->ErrorData);
        break;
        case CPUError::SystemRead:
          System.print(F("SYSRE : "));
          System.print(Emulator->ErrorData);
        break;
        case CPUError::RomWrite:
          System.print(F("ROMWR : "));
          System.print(Emulator->ErrorData);
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
