#include "Chip8Emulator.h"

void Chip8Emulator::ExecuteInstruction(Arduboy2 & System)
{
  const uint8_t highByte = this->ReadMemory(this->ProgramCounter + 0);
  const uint8_t lowByte = this->ReadMemory(this->ProgramCounter + 1);
  this->ProgramCounter += 2;

  const uint16_t instruction = (highByte << 8) | (lowByte << 0);
  const uint8_t opcode = ((highByte >> 4) & 0x0F);
  const uint8_t operandX = ((highByte >> 0) & 0x0F);
  const uint8_t operandY = ((lowByte >> 4) & 0x0F);
  const uint8_t nibble = ((lowByte >> 0) & 0x0F);
  const uint16_t address = (instruction & 0x0FFF);

  switch(opcode)
  {
    case 0x0:  //0x00XX - misc routines
      switch(lowByte)
      {
        case 0xE0:  //CLS - Clear screen
          System.clear();
          return;

        case 0xEE:  //RTS - return from sub
          this->ProgramCounter = this->PullWord();
          return;

        case 0xFD:  //EXIT - End program
          this->Halt();
          return;
      }
      break;

    case 0x1:  //JUMP to xNNN
      this->ProgramCounter = address;
      return;

    case 0x2:  //CALL
      this->PushWord(this->ProgramCounter);
      this->ProgramCounter = address;
      return;

    case 0x3:  //SKE - Skip if selected register = lowByte byte
      if(this->Register[operandX] == lowByte)
      {
        this->ProgramCounter += 2;
      }
      return;

    case 0x4:  //SKNE - Skip if reg != lowByte byte
      if(this->Register[operandX] != lowByte)
      {
        this->ProgramCounter += 2;
      }
      return;

    case 0x5:  //??? - Skip if operandY register == selected register
      if(this->Register[operandY] == this->Register[operandX])
      {
        this->ProgramCounter += 2;
      }
      return;

    case 0x6: //LOAD - store constant into register
      this->Register[operandX] = lowByte;
      return;

    case 0x7:  //ADD - Add value to register
      this->Register[operandX] += lowByte;
      return;

    case 0x8:  //Numerical operations and stuff
      switch(nibble)
      {
        case 0x0: //LOAD
          this->Register[operandX] = this->Register[operandY];
          return;

        case 0x1: //OR
          this->Register[operandX] |= this->Register[operandY];
          return;

        case 0x2: //AND
          this->Register[operandX] &= this->Register[operandY];
          return;

        case 0x3: //XOR
          this->Register[operandX] ^= this->Register[operandY];
          return;

        case 0x4: //ADD
          {
            uint16_t result = (this->Register[operandX] + this->Register[operandY]);
            this->Register[0xF] = (result > 0xFF) ? 1 : 0;
            this->Register[operandX] = (result & 0xFF);
          }
          return;

        case 0x5: //SUB - Subtract operandY from operandX, if borrow store 0 in reg 0xF
          this->Register[0xF] = this->Register[operandX] > this->Register[operandY];
          this->Register[operandX] -= this->Register[operandY];
          return;

        case 0x6: //SHR - Shift bits right. Bit 0 goes into reg 0xF
          this->Register[0xF] = this->Register[operandX] & 0x1;
          this->Register[operandX] >>= 1;
          return;

        case 0x7: //SUBN  - Subtract operandY from operandX, is borrow store 1 in reg 0xF
          this->Register[0xF] = this->Register[operandY] > this->Register[operandX];
          this->Register[operandX] = (this->Register[operandY] - this->Register[operandX]);
          return;

        case 0xE: //SHL - Shift bits left. Bit 7 goes into reg 0xF
          this->Register[0xF] = (this->Register[operandX] >> 7) & 0x1;
          this->Register[operandX] <<= 1;
          return;
      }
      break;

    case 0x9:  //SKNE - Skip if operandY reg != operandX reg
      if(this->Register[operandY] != this->Register[operandX])
      {
        this->ProgramCounter += 2;
      }
      return;

    case 0xA:  //LOAD - load index reg with data
      this->Index = address;
      return;

    case 0xB:  //JUMP + i - PC goes moved to immediate address + V0
      this->ProgramCounter = (address + this->Register[0]);
      return;

    case 0xC:  //RAND - generated random number &'d with lowByte byte, store in selected register
      this->Register[operandX] = (rand() & lowByte);
      return;

    case 0xD:
      {
        // DXYN
        // D - sprite draw
        // X - X position register
        // Y - Y position register
        // N - Sprite height

        const uint8_t x = this->Register[operandX];
        const uint8_t y = this->Register[operandY];

        const uint8_t height = nibble;
        uint8_t collision = 0;

        for(uint8_t yIndex = 0; yIndex < height; ++yIndex)
        {
          const uint8_t drawY = (y + yIndex) % 32;
          const uint8_t spriteData = this->ReadMemory(this->Index + yIndex);

          for(uint8_t xIndex = 0; xIndex < 8; ++xIndex)
          {
            // Skip pixels with a value of 0
            if(((spriteData >> xIndex) & 0x1) == 0)
              continue;

            const int8_t drawX = (x + (7 - xIndex)) % 64;
            const uint8_t pixel = System.getPixel(drawX, drawY);

            if (pixel != 0) //If surface pixel is on
              collision = 1; //Collision on

            System.drawPixel(drawX, drawY, ~pixel);  //invert drawn pixel
          }
        }

        this->Register[0xF] = collision;
      }
      return;

    case 0xE:  //Input stuff
      switch(lowByte)
      {
        case 0x9E:
          return;

        case 0xA1:
          return;
      }
      break;

    case 0xF: // IO stuff
      switch(lowByte)
      {
        case 0x07: //Load delay - timer -> register
          this->Register[operandX] = this->DelayTimer;
          return;

        case 0x0A:  //KEYD - Pause until key pressed
          this->Mode = CPUMode::InputWait;
          this->InputRegister = operandX;
          return;

        case 0x15:  //Store delay - register -> timer
          this->DelayTimer = this->Register[operandX];
          return;

        case 0x18:  //Store sound - register -> timer
          {
            this->SoundTimer = this->Register[operandX];

            if(this->SoundTimer > 0)
              BeepPin1::tone(BeepPin1::freq(Chip8Emulator::SoundFrequency));
            else
              BeepPin1::noTone();
          }
          return;

        case 0x1E:  //Add I
          this->Index += this->Register[operandX];
          return;

        case 0x29:  //Load sprite index
          this->Index = this->Register[operandX] * 5;
          return;

        case 0x33:  //BCD
          {
            uint8_t value = this->Register[operandX];

            for(uint8_t offset = 3; offset > 0; --offset, value /= 10)
            {
              const uint8_t digit = (value % 10);
              this->WriteMemory(this->Index + (offset - 1), digit);
            }
          }
          return;

        case 0x55:  //STORE I - Store n registers into memory[index]
          for(uint8_t i = 0; i <= operandX; ++i)
          {
            this->WriteMemory(this->Index + i, this->Register[i]);
          }
          return;

        case 0x65:  //LOAD I - Load n number of registers from memory[index]
          for(uint8_t i = 0; i <= operandX; ++i)
          {
            this->Register[i] = this->ReadMemory(this->Index + i);
          }
          return;

        case 0x75:  //SRPL - Move N registers to temp
          for(uint8_t i = 0; i <= operandX; ++i)
          {
            this->RegisterTemp[i] = this->Register[i];
          }
          return;

        case 0x85:  //LRPL - Move N temp registers to main
          for(uint8_t i = 0; i <= operandX; ++i)
          {
            this->Register[i] = this->RegisterTemp[i];
          }
          return;
      }
      break;
  }

  this->Error(CPUError::UnknownOpcode, instruction);
}

MemoryPartition Chip8Emulator::GetMemoryPartition(const size_t Location) const
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

uint8_t Chip8Emulator::ReadMemory(const size_t Location)
{
#if SMALL_MEMORY
  // Font Space
  if(Location < FontDataSize)
  {
    return static_cast<uint8_t>(pgm_read_byte(&FontData[Location]));
  }

  // System Space
  if(Location < this->RomStart)
  {
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
    return 0;
  }

  if(Location > 4096)
  {
    this->Error(CPUError::ExternalRead, Location);
    return 0;
  }

  return this->Memory[Location - this->RomEnd];
#else
  return this->Memory[Location];
#endif
}

void Chip8Emulator::WriteMemory(const size_t Location, const uint8_t Value)
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

void Chip8Emulator::PushWord(const uint16_t Word)
{
  if(this->StackPointer >= this->StackSize)
  {
    this->Error(CPUError::StackOverflow);
    return;
  }

  this->Stack[this->StackPointer] = Word;
  ++this->StackPointer;
}

uint16_t Chip8Emulator::PullWord()
{
  if(this->StackPointer == this->StackStart)
  {
    this->Error(CPUError::StackUnderflow);
    return 0;
  }

  --this->StackPointer;
  return this->Stack[this->StackPointer];
}

void Chip8Emulator::Error(CPUError ErrorType, uint16_t ErrorData)
{
  this->Mode = CPUMode::Error;
  this->ErrorType = ErrorType;
  this->ErrorData = ErrorData;
}

Chip8Emulator::Chip8Emulator(const uint8_t * Rom, const size_t RomSize)
{
  this->Load(Rom, RomSize);
}

void Chip8Emulator::Load(const uint8_t * Rom, const size_t RomSize)
{
  //Configure rom
  this->Rom = Rom;
  this->RomSize = RomSize;
  this->RomStart = 0x200;
  this->RomEnd = this->RomStart + RomSize;
  this->Reset();
}

void Chip8Emulator::Tick(Arduboy2 & System)
{
  if(this->Mode == CPUMode::InputWait)
  {
    if(this->InputPressed)  //If key pressed this frame
    {
      System.clear();
      this->Register[this->InputRegister] == this->InputLast;
      this->Mode == CPUMode::Running;
    }
  }
  if(this->Mode != CPUMode::Running)
    return;

  ExecuteInstruction(System);
}

void Chip8Emulator::Tick(Arduboy2 & System, uint8_t Repeat)
{
  for(auto i = 0; i < Repeat; ++i)
  {
    this->Tick(System);
  }
}

void Chip8Emulator::SendInput(const uint8_t KeyID)
{
  this->InputPressed = true;
  this->InputLast = KeyID;
}

void Chip8Emulator::UpdateDelayTimer()
{
  if(this->DelayTimer > 0)
    --this->DelayTimer;
}

void Chip8Emulator::UpdateSoundTimer()
{
  if(this->SoundTimer > 0)
    --this->SoundTimer;

  if(this->SoundTimer == 0)
    BeepPin1::noTone();
}

void Chip8Emulator::UpdateInput()
{
  this->InputPressed = false;
}

void Chip8Emulator::Halt(void)
{
  this->Mode = CPUMode::Stopped;
}
void Chip8Emulator::Reset(void)
{
  //bootup
  for(size_t i = 0; i < 16; ++i)
  {
      this->Register[i] = 0;
      this->RegisterTemp[i] = 0;
  }
  this->DelayTimer = 0;
  this->SoundTimer = 0;
  this->Index = 0;
  this->ProgramCounter = this->RomStart;
  this->StackPointer = this->StackStart;
  this->Mode = CPUMode::Running;
}
