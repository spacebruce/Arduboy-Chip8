#include "debugmodule.h"

DebugModule::DebugModule(Chip8Emulator & Emulator)
{
  this->Emulator = &Emulator;
}

void DebugModule::Tick(Arduboy2 & System)
{
  size_t Address = Emulator->ProgramCounter;
  CurrentOpcode = (Emulator->ReadMemory(Address) << 8) + Emulator->ReadMemory(Address + 1);

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

  switch(this->ViewMode)
  {
    case DebugScreenView::Memory8:
    case DebugScreenView::Memory16:
      {
        if(System.pressed(UP_BUTTON))
        {
          if(this->Scroll > 0)
            --this->Scroll;
        }

        if(System.pressed(DOWN_BUTTON))
        {
          this->Scroll = (this->Scroll + 1);
        }
      }
      break;

    case DebugScreenView::Input:
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
        if(System.justPressed(A_BUTTON))
        {
          Emulator->SendInput(this->InputSelected);
        }
      }
      break;
  }
}

void DebugModule::Draw(Arduboy2 & System)
{
  this->ClearDebugArea();

  // Memory viewer
  switch(this->ViewMode)
  {
    case DebugScreenView::Memory8:
      this->DrawMemory8View(System);
      break;

    case DebugScreenView::Memory16:
      this->DrawMemory16View(System);
      break;

    case DebugScreenView::Processor:
      this->DrawProcessorView(System);
      break;

    case DebugScreenView::Stack:
      this->DrawStackView(System);
      break;

    case DebugScreenView::Input:
      this->DrawInputView(System);
      break;
  }

  //Status message
  System.setCursor(0,56);
  switch(Emulator->Mode)
  {
    case CPUMode::Error:
      this->DrawError(System);
      break;

    case CPUMode::Stopped:
      System.print(F("CPU HALT"));
      break;

    case CPUMode::Startup:
      System.print(F("CPU STARTUP"));
      break;

    case CPUMode::InputWait:
      System.print(F("WAITING FOR INPUT"));
      break;
  }
}

void DebugModule::ClearDebugArea()
{
  constexpr size_t screenWidth = WIDTH;
  constexpr size_t screenHeight = HEIGHT;
  constexpr size_t halfScreenWidth = (screenWidth / 2);
  constexpr size_t halfScreenHeight = (screenHeight / 2);

  constexpr size_t columnHeight = 8;
  constexpr size_t halfVerticalColumns = (halfScreenHeight / columnHeight);

  for(size_t y = 0; y < halfVerticalColumns; ++y)
    for(size_t x = 0; x < halfScreenWidth; ++x)
    {
      const size_t index = ((y * screenWidth) + x);
      Arduboy2::sBuffer[halfScreenWidth + index] = 0;
    }

  constexpr size_t screenSize = ((screenWidth * screenHeight) / 8);
  constexpr size_t halfScreenSize = (screenSize / 2);

  for(size_t index = 0; index < halfScreenSize; ++index)
    Arduboy2::sBuffer[halfScreenSize + index] = 0;
}

void DebugModule::DrawMemory8View(Arduboy2 & System)
{
  System.setCursor(65, 0);
  System.println(F("8 bit"));

  uint8_t line = 1;
  for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
  {
    System.setCursor(70, line * 8);
    ++line;
    System.print(i);
    System.print(F("|"));
    System.println(Emulator->ReadMemory(i), HEX);
  }
}

void DebugModule::DrawMemory16View(Arduboy2 & System)
{
  System.setCursor(65, 0);
  System.println(F("16 bit"));

  size_t pointer = floor(this->Scroll / 2) * 2;

  uint8_t line = 1;
  for(size_t i = this->Scroll; i < this->Scroll + 7; ++i)
  {
    System.setCursor(70, line * 8);
    ++line;
    System.print(pointer);
    System.print(F("|"));

    int16_t value = (Emulator->ReadMemory(pointer) << 8) + (Emulator->ReadMemory(pointer + 1));
    pointer += 2;
    System.println(value, HEX);
  }
}

void DebugModule::DrawProcessorView(Arduboy2 & System)
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
  System.print(F("DT:"));  System.print(Emulator->DelayTimer);

  System.setCursor(80,32);
  System.print(F("ST:"));  System.print(Emulator->SoundTimer);

  System.setCursor(0,40);
  for(uint8_t i = 0; i < 8; ++i)
  {
    System.setCursor(i*16, 40);
    System.print(Emulator->Register[i], HEX);
    System.setCursor(i*16, 48);
    System.print(Emulator->Register[i+8], HEX);
  }
}

void DebugModule::DrawStackView(Arduboy2 & System)
{
  System.setCursor(65,0);
  System.print(F("STACK VIEW"));
  System.setCursor(65,8);
  System.print(F("SP"));
  System.print(Emulator->StackPointer);

  for(uint8_t i = 0; i < Chip8Emulator::StackSize; ++i)
  {
    const uint8_t Row = (i >= Chip8Emulator::StackSize / 2) ? 48 : 40;
    const int16_t X = (i % ((Chip8Emulator::StackSize / 2) - 1) * 16);
    System.setCursor(X, Row);
    System.print(Emulator->Stack[i], HEX);
  }
}

void DebugModule::DrawInputView(Arduboy2 & System)
{
  for(uint8_t i = 0; i <= 0xF; ++i)
  {
    const uint8_t Row = (i >= 0x8) ? 48 : 40;
    const int16_t X = (i % (0x8) * 16);
    System.setCursor(X, Row);
    System.print(i, HEX);
    if(this->InputSelected == i)
      System.print(F("<"));
  }
}

void DebugModule::DrawError(Arduboy2 & System)
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
