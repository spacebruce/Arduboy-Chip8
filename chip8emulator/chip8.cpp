#include "chip8.h"

void Chip8::ExecuteInstruction(Arduboy2* System)
{
  uint8_t High = this->ReadMemory(this->ProgramCounter);
  uint8_t Low = this->ReadMemory(this->ProgramCounter + 1);
  uint16_t Opcode = (High << 8) | Low;
  this->ProgramCounter += 2;

  bool unknown = false; //error decoding opcode
  uint16_t location;
  uint8_t byteX = High & 0x0F;
  uint8_t byteY = (Low & 0xF0) >> 4;
  switch(High & 0xF0)
  {
    case 0x00:  //0x00XX - misc routines
      switch(Low)
      {
        case 0xCF:  //Scroll down
        break;
        case 0xE0:  //CLS - Clear screen
          System->clear();
        break;
        case 0xEE:  //RTS - return from sub
          this->ProgramCounter = (this->ReadMemory(this->StackPointer - 1) << 8) + this->ReadMemory(this->StackPointer - 2);
          this->StackPointer -= 2;
        break;
        case 0xFB:  //SCRR - Scroll right
        break;
        case 0xFC:  //SCRL - Scroll left
        break;
        case 0xFD:  //EXIT - End program
          this->Halt();
        break;
        default:
          unknown = true;
        break;
      }
  break;
  case 0x10:  //JUMP to xNNN
    this->ProgramCounter = Opcode & 0x0FFF;
  break;
  case 0x20:  //CALL
    this->WriteMemory(this->StackPointer + 0, static_cast<uint8_t>(this->ProgramCounter & 0xFF));
    this->WriteMemory(this->StackPointer + 1, static_cast<uint8_t>((this->ProgramCounter >> 8) & 0xFF));
    this->StackPointer += 2;
    this->ProgramCounter = Opcode & 0x0FFF;
  break;
  case 0x30:  //SKE - Skip if selected register = low byte
    if(this->Register[byteX] == Low)
    {
      this->ProgramCounter += 2;
    }
  break;
  case 0x40:  //SKNE - Skip if reg != low byte
    if(this->Register[byteX] != Low)
    {
      this->ProgramCounter += 2;
    }
  break;
  case 0x50:  //??? - Skip if byteY register == selected register
    if(this->Register[byteY] == this->Register[byteX])
    {
      this->ProgramCounter += 2;
    }
  break;
  case 0x60: //LOAD - store constant into register
    this->Register[byteX] = Low;
  break;
  case 0x70:  //ADD - Add value to register
    this->Register[byteX] += Low;
  break;
  case 0x80:  //Numerical operations and stuff
    switch(Low & 0x0F)
    {
      case 0x0: //LOAD
        this->Register[byteX] = this->Register[byteY];
      break;
      case 0x1: //OR
        this->Register[byteX] = (this->Register[byteX] | this->Register[byteY]);
      break;
      case 0x2: //AND
        this->Register[byteX] = (this->Register[byteX] & this->Register[byteY]);
      break;
      case 0x3: //XOR
        this->Register[byteX] = (this->Register[byteX] ^ this->Register[byteY]);
      break;
      case 0x4: //ADD
        this->Register[byteX] = (this->Register[byteX] + this->Register[byteY]);
      break;
      case 0x5: //SUB - Subtract byteY from byteX, if borrow store 0 in reg 0xF
        this->Register[0xF] = this->Register[byteX] > this->Register[byteY];
        this->Register[byteX] -= this->Register[byteY];
      break;
      case 0x6: //SHR - Shift bits right. Bit 0 goes into reg 0xF
        this->Register[0xF] = this->Register[byteY] & 0x1;
        this->Register[byteY] = this->Register[byteY] >> 1;
      break;
      case 0x7: //SUBN  - Subtract byteY from byteX, is borrow store 1 in reg 0xF
        this->Register[0xF] = this->Register[byteY] > this->Register[byteX];
        this->Register[byteX] -= this->Register[byteY];
      break;
      case 0xE: //SHL - Shift bits left. Bit 7 goes into reg 0xF
        this->Register[0xF] = (this->Register[byteY] >> 7) & 0x1;
        this->Register[byteY] = this->Register[byteY] << 1;
      break;
      default:
        unknown = true;
      break;
    }
  break;
  case 0x90:  //SKNE - Skip if byteY reg != byteX reg
    if(this->Register[byteY] != this->Register[byteX])
    {
      this->ProgramCounter += 2;
    }
  break;
  case 0xA0:  //LOAD - load index reg with data
    this->Index = Opcode & 0x0FFF;
  break;
  case 0xB0:  //JUMP + i - PC goes moved to Index + operand const
    this->ProgramCounter = (ReadMemory(this->Register[0]) << 8) + (ReadMemory(this->Register[0]+1)) + Opcode & 0x0FFF;
  break;
  case 0xC0:  //RAND - generated random number &'d with low byte, store in selected register
    this->Register[byteX] = random(255) & Low;
  break;
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
    for(uint8_t drawY = 0; drawY < height; ++drawY)
    {
      uint8_t sprite = ReadMemory(this->Index + drawY);
      for(uint8_t drawX = 0; drawX < 8; ++drawX)
      {
        if((sprite >> drawX) & 0x1) //If current pixel in sprite is on
        {
          bool enabled = System->getPixel(x + (7 - drawX), y + drawY);
          if (enabled) //If surface pixel is on
            this->Register[0xF] = 1; //Collision on
          System->drawPixel((x + (7 - drawX)) % 64, (y + drawY) % 32, !enabled);  //invert drawn pixel
        }
      }
    }
  break;
  case 0xE0:  //Input stuff
    switch(Low)
    {
      case 0x9E:
      break;
      case 0xA1:
      break;
      default:
        unknown = true;
      break;
    }
  break;
  case 0xF0: // IO stuff
    switch(Low)
    {
      case 0x07: //Load delay - timer -> register
        this->Register[byteX] = this->TimerDelay;
      break;
      case 0x0A:  //KEYD - Pause until key pressed
        //to do
        this->Register[byteX] = 0;
      break;
      case 0x15:  //Store delay - register -> timer
        this->TimerDelay = this->Register[byteX];
      break;
      case 0x18:  //Store sound - register -> timer
        this->TimerSound = this->Register[byteX];
      break;
      case 0x1E:  //Add I
        this->Index += this->Register[byteY];
      break;
      case 0x29:  //Load sprite index
        this->Index = this->Register[byteY] * 5;
      break;
      case 0x33:  //BCD
        //not today, satan
      break;
      case 0x55:  //STORE I - Store n registers into memory[index]
        location = this->Index;
        for(uint8_t i = 0; i <= High & 0xF; ++i)
        {
          this->WriteMemory(byteX + i, this->Register[i]);
        }
      break;
      case 0x65:  //LOAD I - Load n number of registers from memory[index]
        location = this->Index;
        for(uint8_t i = 0; i <= High & 0xF; ++i)
        {
          this->Register[i] = this->ReadMemory(byteX + i);
        }
      break;
      case 0x75:  //SRPL - Move N registers to temp
        for(uint8_t i = 0; i <= byteX; ++i)
        {
          this->RegisterTemp[i] = this->Register[i];
        }
      break;
      case 0x85:  //LRPL - Move N temp registers to main
        for(uint8_t i = 0; i <= byteX; ++i)
        {
          this->Register[i] = this->RegisterTemp[i];
        }
      break;
      default:
        unknown = true;
      break;
    }
    break;
    default:
      unknown = true;
    break;
  }

  if(unknown)
  {
    this->Mode = CPUMode::Error;
    this->Error = CPUError::UnknownOpcode;
    this->ErrorData = Opcode;
  }
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
  if(Location < this->RomStart) //empty space
  {
    if(Location < FontDataSize) //font space
    {
      return static_cast<uint8_t>(pgm_read_byte(&FontData[Location]));
    }
    else if ((Location >= StackStart) && (Location <= StackStart + StackSize))
    {
      return this->Stack[Location - StackStart];
    }
    else
    {
      return 0;
    }
  }
  if((Location >= this->RomStart) && (Location < this->RomEnd)) //Static rom space
  {
    size_t offset = Location - this->RomStart;
    return static_cast<uint8_t>(pgm_read_byte(&(this->Rom[offset])));
  }
  else  //RAM
  {
    if(Location > 4096)
    {
      this->Mode = CPUMode::Error;
      this->Error = CPUError::MemoryRead;
      this->ErrorData = Location;
    }
    return this->Memory[Location - this->RomEnd];
  }
#else
  return this->Memory[Location];
#endif
}

void Chip8::WriteMemory(const size_t Location, const uint8_t Value)
{
#if SMALL_MEMORY
  bool valid = true;
  if((Location >= this->RomEnd))
  {
    this->Memory[Location] = Value;
  }
  else if ((Location >= StackStart) && (Location <= StackStart + StackSize))
  {
    return Stack[Location - StackStart] = Value;
  }
  else
  {
    this->Mode = CPUMode::Error;
    this->Error = CPUError::MemoryWrite;
    this->ErrorData = Location;
  }
#else
  this->Memory[Location] = Value;
#endif
}


Chip8::Chip8(uint8_t* Rom, const size_t RomSize)
{
  this->Load(Rom, RomSize);
}

void Chip8::Load(uint8_t* Rom, const size_t RomSize)
{
  //Configure rom
  this->Rom = Rom;
  this->RomSize = RomSize;
  this->RomStart = 0x200;
  this->RomEnd = this->RomStart + RomSize;
  this->Reset();
}

void Chip8::Tick(Arduboy2* System, uint8_t Repeat = 50)
{
  for(auto i = 0; i < Repeat; ++i)
    ExecuteInstruction(System);
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
