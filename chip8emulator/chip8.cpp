#include "chip8.h"

void Chip8::ExecuteInstruction(Arduboy2 & System)
{
  uint8_t High = this->ReadMemory(this->ProgramCounter);
  uint8_t Low = this->ReadMemory(this->ProgramCounter + 1);
  uint16_t Opcode = (High << 8) | Low;
  this->ProgramCounter += 2;

  uint16_t location;
  uint8_t byteX = High & 0x0F;
  uint8_t byteY = (Low & 0xF0) >> 4;
  switch(High & 0xF0)
  {
    case 0x00:  //0x00XX - misc routines
      switch(Low)
      {
        case 0xE0:  //CLS - Clear screen
          System.clear();
        return;
        case 0xEE:  //RTS - return from sub
          this->ProgramCounter = this->PullWord();
        break;
        case 0xFD:  //EXIT - End program
          this->Halt();
        return;
      }
  break;
  case 0x10:  //JUMP to xNNN
    this->ProgramCounter = Opcode & 0x0FFF;
  return;
  case 0x20:  //CALL
    this->PushWord(this->ProgramCounter);
    this->ProgramCounter = Opcode & 0x0FFF;
  return;
  case 0x30:  //SKE - Skip if selected register = low byte
    if(this->Register[byteX] == Low)
    {
      this->ProgramCounter += 2;
    }
  return;
  case 0x40:  //SKNE - Skip if reg != low byte
    if(this->Register[byteX] != Low)
    {
      this->ProgramCounter += 2;
    }
  return;
  case 0x50:  //??? - Skip if byteY register == selected register
    if(this->Register[byteY] == this->Register[byteX])
    {
      this->ProgramCounter += 2;
    }
  return;
  case 0x60: //LOAD - store constant into register
    this->Register[byteX] = Low;
  return;
  case 0x70:  //ADD - Add value to register
    this->Register[byteX] += Low;
  return;
  case 0x80:  //Numerical operations and stuff
    switch(Low & 0x0F)
    {
      case 0x0: //LOAD
        this->Register[byteX] = this->Register[byteY];
      return;
      case 0x1: //OR
        this->Register[byteX] = (this->Register[byteX] | this->Register[byteY]);
      return;
      case 0x2: //AND
        this->Register[byteX] = (this->Register[byteX] & this->Register[byteY]);
      return;
      case 0x3: //XOR
        this->Register[byteX] = (this->Register[byteX] ^ this->Register[byteY]);
      return;
      case 0x4: //ADD
        this->Register[byteX] = (this->Register[byteX] + this->Register[byteY]);
      return;
      case 0x5: //SUB - Subtract byteY from byteX, if borrow store 0 in reg 0xF
        this->Register[0xF] = this->Register[byteX] > this->Register[byteY];
        this->Register[byteX] -= this->Register[byteY];
      return;
      case 0x6: //SHR - Shift bits right. Bit 0 goes into reg 0xF
        this->Register[0xF] = this->Register[byteY] & 0x1;
        this->Register[byteY] = this->Register[byteY] >> 1;
      return;
      case 0x7: //SUBN  - Subtract byteY from byteX, is borrow store 1 in reg 0xF
        this->Register[0xF] = this->Register[byteY] > this->Register[byteX];
        this->Register[byteX] -= this->Register[byteY];
      return;
      case 0xE: //SHL - Shift bits left. Bit 7 goes into reg 0xF
        this->Register[0xF] = (this->Register[byteY] >> 7) & 0x1;
        this->Register[byteY] = this->Register[byteY] << 1;
      return;
    }
  break;
  case 0x90:  //SKNE - Skip if byteY reg != byteX reg
    if(this->Register[byteY] != this->Register[byteX])
    {
      this->ProgramCounter += 2;
    }
  return;
  case 0xA0:  //LOAD - load index reg with data
    this->Index = Opcode & 0x0FFF;
  return;
  case 0xB0:  //JUMP + i - PC goes moved to immediate address + V0
    this->ProgramCounter = (Opcode & 0x0FFF) + this->Register[0];
  return;
  case 0xC0:  //RAND - generated random number &'d with low byte, store in selected register
    this->Register[byteX] = random(255) & Low;
  return;
  case 0xD0:
    /*  DXXX
      D - sprite draw
      X - X position register
      X - Y position register
      X - Sprite height
    */
    uint8_t x = this->Register[byteX];//this->Register[High & 0x0F];
    uint8_t y = this->Register[byteY];//this->Register[(Low & 0xF0) >> 4];
    uint8_t height = (Low & 0x0F);
    this->Register[0xF] = 0;  //Collision register
    uint8_t screenX, screenY;
    for(uint8_t drawY = 0; drawY < height; ++drawY)
    {
      screenY = (y + drawY) % 32;
      uint8_t sprite = this->ReadMemory(this->Index + drawY);
      for(uint8_t drawX = 0; drawX < 8; ++drawX)
      {
        if((sprite >> drawX) & 0x1) //If current pixel in sprite is on
        {
          screenX = (x + (7 - drawX)) % 64;
          bool enabled = System.getPixel(screenX, screenY);
          if (enabled) //If surface pixel is on
            this->Register[0xF] = 1; //Collision on
          System.drawPixel(screenX, screenY, !enabled);  //invert drawn pixel
        }
      }
    }
  return;
  case 0xE0:  //Input stuff
    switch(Low)
    {
      case 0x9E:
      return;
      case 0xA1:
      return;
    }
  break;
  case 0xF0: // IO stuff
    switch(Low)
    {
      case 0x07: //Load delay - timer -> register
        this->Register[byteX] = this->TimerDelay;
      return;
      case 0x0A:  //KEYD - Pause until key pressed
        //to do
        this->Register[byteX] = 0;
      return;
      case 0x15:  //Store delay - register -> timer
        this->TimerDelay = this->Register[byteX];
      return;
      case 0x18:  //Store sound - register -> timer
        this->TimerSound = this->Register[byteX];
      return;
      case 0x1E:  //Add I
        this->Index += this->Register[byteY];
      return;
      case 0x29:  //Load sprite index
        this->Index = this->Register[byteY] * 5;
      return;
      case 0x33:  //BCD
        //not today, satan
      return;
      case 0x55:  //STORE I - Store n registers into memory[index]
        for(uint8_t i = 0; i <= byteX; ++i)
        {
          this->WriteMemory(this->Index + i, this->Register[i]);
        }
      return;
      case 0x65:  //LOAD I - Load n number of registers from memory[index]
        for(uint8_t i = 0; i <= byteX; ++i)
        {
          this->Register[i] = this->ReadMemory(this->Index + i);
        }
      return;
      case 0x75:  //SRPL - Move N registers to temp
        for(uint8_t i = 0; i <= byteX; ++i)
        {
          this->RegisterTemp[i] = this->Register[i];
        }
      return;
      case 0x85:  //LRPL - Move N temp registers to main
        for(uint8_t i = 0; i <= byteX; ++i)
        {
          this->Register[i] = this->RegisterTemp[i];
        }
      return;
    }
    break;
  }

  this->Error(CPUError::UnknownOpcode, Opcode);
}

MemoryPartition Chip8::GetMemoryPartition(const size_t Location) const
{
  if(Location < this->RomStart) //empty space
  {
    return MemoryPartition::Static;
  }
  if((Location >= this->RomStart) && (Location < this->RomEnd)) //Static rom space
  {
    return MemoryPartition::ROM;
  }
  else  //RAM
  {
    return MemoryPartition::RAM;
  }
}

uint8_t Chip8::ReadMemory(const size_t Location)
{
#if SMALL_MEMORY
  // System Space
  if(Location < this->RomStart)
  {
    // Font Space
    if(Location < FontDataSize)
    {
      return static_cast<uint8_t>(pgm_read_byte(&FontData[Location]));
    }

    this->Error(CPUError::SystemRead, Location);

    return 0;
  }

  // ROM
  if(Location < this->RomEnd)
  {
    size_t offset = Location - this->RomStart;
    return static_cast<uint8_t>(pgm_read_byte(&(this->Rom[offset])));
  }

  // RAM
  if((Location - this->RomEnd) > MemorySize)
  {
    this->Error(CPUError::AbsentRead, Location);
    return;
  }

  if(Location > 4096)
  {
    this->Error(CPUError::ExternalRead, Location);
  }

  return this->Memory[Location - this->RomEnd];
#else
  return this->Memory[Location];
#endif
}

void Chip8::WriteMemory(const size_t Location, const uint8_t Value)
{
#if SMALL_MEMORY

  if(Location < this->RomStart)
  {
    this->Error(CPUError::SystemWrite, Location);
    return;
  }

  if(Location < this->RomEnd)
  {
    this->Error(CPUError::RomWrite, Location);
    return;
  }

  if(Location > 4096)
  {
    this->Error(CPUError::ExternalWrite, Location);
    return;
  }

  if((Location - this->RomEnd) > MemorySize)
  {
    this->Error(CPUError::AbsentWrite, Location);
    return;
  }

  this->Memory[Location - this->RomEnd] = Value;
#else
  this->Memory[Location] = Value;
#endif
}

void Chip8::PushWord(const uint16_t Word)
{
  if(this->StackPointer >= this->StackSize)
  {
    this->Error(CPUError::StackOverflow);
    return;
  }

  this->Stack[this->StackPointer] = Word;
  ++this->StackPointer;
}

uint16_t Chip8::PullWord()
{
  if(this->StackPointer == this->StackStart)
  {
    this->Error(CPUError::StackUnderflow);
    return 0;
  }

  --this->StackPointer;
  return this->Stack[this->StackPointer];
}

void Chip8::Error(CPUError ErrorType, uint16_t ErrorData)
{
  this->Mode = CPUMode::Error;
  this->ErrorType = ErrorType;
  this->ErrorData = ErrorData;
}

Chip8::Chip8(const uint8_t * Rom, const size_t RomSize)
{
  this->Load(Rom, RomSize);
}

void Chip8::Load(const uint8_t * Rom, const size_t RomSize)
{
  //Configure rom
  this->Rom = Rom;
  this->RomSize = RomSize;
  this->RomStart = 0x200;
  this->RomEnd = this->RomStart + RomSize;
  this->Reset();
}

void Chip8::Tick(Arduboy2 & System)
{
    if(this->Mode != CPUMode::Running)
      return;

    ExecuteInstruction(System);
}

void Chip8::Tick(Arduboy2 & System, uint8_t Repeat)
{
  for(auto i = 0; i < Repeat; ++i)
  {
    if(this->Mode != CPUMode::Running)
      return;

    ExecuteInstruction(System);
  }
}

void Chip8::Halt(void)
{
  this->Mode = CPUMode::Stopped;
}
void Chip8::Reset(void)
{
  Serial.println(F("Reset CPU..."));
  //bootup
  for(size_t i = 0; i < 16; ++i)
  {
      this->Register[i] = 0;
      this->RegisterTemp[i] = 0;
  }
  this->TimerDelay = 0;
  this->TimerSound = 0;
  this->Index = 0;
  this->ProgramCounter = this->RomStart;
  this->StackPointer = this->StackStart;
  this->Mode = CPUMode::Running;
}
