#include "chip8.h"

void Chip8::ExecuteInstruction()
{
  uint8_t High = this->ReadMemory(this->ProgramCounter);
  uint8_t Low = this->ReadMemory(this->ProgramCounter + 1);
  uint16_t Opcode = (High << 8) + Low;
  this->ProgramCounter += 2;

  uint8_t target, source;
  uint16_t location;
  switch(High & 0xF0)
  {
    case 0x00:  //0x00XX - misc routines
      switch(Low)
      {
        case 0xCF:  //Scroll down
        break;
        case 0xE0:  //CLS - Clear screen
        break;
        case 0xEE:  //RTS - return from sub
        break;
        case 0xFB:  //SCRR - Scroll right
        break;
        case 0xFC:  //SCRL - Scroll left
        break;
        case 0xFD:  //EXIT - End program
          this->Halt();
        break;
        case 0xFE:  //EXTD - Disable extended mode
        break;
        case 0xFF:  //EXTE - Enabled extended mode
        break;
      }
  break;
  case 0x10:  //JUMP to xNNN
    this->ProgramCounter = Opcode & 0x0FFF;
  break;
  case 0x20:  //CALL
    //
  break;
  case 0x30:  //SKE - Skip if selected register = low byte
    target = High & 0xF;
    if(this->Register[target] == Low)
    {
      this->ProgramCounter += 2;
    }
  break;
  case 0x40:  //SKNE - Skip if reg != low byte
    target = High & 0xF;
    if(this->Register[target] != Low)
    {
      this->ProgramCounter += 2;
    }
  break;
  case 0x50:  //??? - Skip if source register == selected register
    source = High & 0xF;
    target = (Low & 0xF0) >> 4;
    if(this->Register[source] == this->Register[target])
      this->ProgramCounter += 2;
  break;
  case 0x60: //LOAD - store constant into register
    target = High & 0xF;
    this->Register[target] = Low;
  break;
  case 0x70:  //ADD - Add value to register
    target = High & 0xF;
    this->Register[target] += Low;
  break;
  case 0x80:  //Numerical operations and stuff
    switch(Low & 0x0F)
    {
      case 0x0: //LOAD
      break;
      case 0x1: //OR
      break;
      case 0x2: //AND
      break;
      case 0x3: //XOR
      break;
      case 0x4: //ADD
      break;
      case 0x5: //SUB
      break;
      case 0x6: //SHR - Shift bits right. Bit 0 goes into reg 0xF
      break;
      case 0x7: //SUBN  - Subtract source from target, is borrow store 1 in reg 0xF
      break;
      case 0xE: //SHL - Shift bits left. Bit 7 goes into reg 0xF
      break;
      default:
      break;
    }
  break;
  case 0x90:  //SKNE - Skip if source reg == target reg
    //????
  break;
  case 0xA0:  //LOAD - load index reg with 0x0FFF
    this->Index = Opcode & 0x0FFF;
  break;
  case 0xB0:  //JUMP + i - PC goes moved to Index + operand const
    this->ProgramCounter = (Opcode & 0x0FFF) + this->Index;
  break;
  case 0xC0:  //RAND - generated random number &'d with low byte, store in selected register
    target = (High & 0xF);
    this->Register[target] = random(255) & Low;
  break;
  case 0xD0:  //DRAW (x,y, )
    //not today, satan
  break;
  case 0xE0:  //Input stuff
    switch(Low)
    {
      case 0x9E:
      break;
      case 0xA1:
      break;
      default:
      break;
    }
  break;
  case 0xF0: // IO stuff
    switch(Low)
    {
      case 0x07: //Load delay - timer -> register
        target = (High & 0xF);
        this->Register[target] = this->TimerDelay;
      break;
      case 0x0A:  //KEYD - Pause until key pressed
        target = (High & 0xF);
        //to do
        this->Register[target] = 0;
      break;
      case 0x15:  //Store delay - register -> timer
        target = (High & 0xF);
        this->TimerDelay = this->Register[target];
      break;
      case 0x18:  //Store sound - register -> timer
        target = (High & 0xF);
        this->TimerSound = this->Register[target];
      break;
      case 0x1E:  //Add I
        source = (High & 0xF);
        this->Index += this->Register[source];
      break;
      case 0x29:  //Load sprite
        source = (High & 0xF);
        this->Index = this->Register[source] * 5;
      break;
      case 0x33:  //BCD
        //not today, satan
      break;
      case 0x55:  //STORE I - Store n registers into memory[index]
        location = this->Index;
        for(uint8_t i = 0; i <= High & 0xF; ++i)
        {
          this->WriteMemory(target + i, this->Register[i]);
        }
      break;
      case 0x65:  //LOAD I - Load n number of registers from memory[index]
        location = this->Index;
        for(uint8_t i = 0; i <= High & 0xF; ++i)
        {
          this->Register[i] = this->ReadMemory(target + i);
        }
      break;
      case 0x75:  //SRPL - Move N registers to temp
        target = High & 0xF;
        for(uint8_t i = 0; i <= target; ++i)
        {
          this->RegisterTemp[i] = this->Register[i];
        }
      break;
      case 0x85:  //LRPL - Move N temp registers to main
        target = High & 0xF;
        for(uint8_t i = 0; i <= target; ++i)
        {
          this->Register[i] = this->RegisterTemp[i];
        }
      break;
      default:
      break;
    }
    break;
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

uint8_t Chip8::ReadMemory(const size_t Location) const
{
#if SMALL_MEMORY
  if(Location < this->RomStart) //empty space
  {
    return 0;
  }
  if((Location >= this->RomStart) && (Location < this->RomEnd)) //Static rom space
  {
    size_t offset = Location - this->RomStart;
    return static_cast<uint8_t>(pgm_read_byte(&(this->Rom[offset])));
  }
  else  //RAM
  {
    return this->Memory[Location - this->RomEnd];
  }
#else
  return this->Memory[Location];
#endif
}

void Chip8::WriteMemory(const size_t Location, const uint8_t Value)
{
#if SMALL_MEMORY
  if((Location >= this->RomEnd))
  {
    this->Memory[Location] = Value;
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

void Chip8::Tick(void)
{
  //for(auto i = 0; i < 50; ++i)
    ExecuteInstruction();
}
void Chip8::Draw(void)
{

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
  this->Mode = CPUMode::Running;
}
