#include "chip8.h"

void Chip8::ExecuteInstruction()
{
  uint16_t Opcode = (this->ReadMemory(this->ProgramCounter) << 8) + (this->ReadMemory(this->ProgramCounter + 1));
  this->ProgramCounter += 2;
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

void Chip8::Tick()
{
  //for(auto i = 0; i < 50; ++i)
    ExecuteInstruction();
}
void Chip8::Draw()
{

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

void Chip8::Halt()
{
  this->Mode = CPUMode::Stopped;
}
void Chip8::Reset()
{
  Serial.println(F("Reset CPU..."));
  //bootup
  for(size_t i = 0; i < 16; ++i)
  {
      this->Register[i] = 0;
      this->Stack[i] = 0;
  }
  this->TimerDelay = 0;
  this->TimerSound = 0;
  this->Index = 0;
  this->ProgramCounter = this->RomStart;
  this->StackPointer = 0;
  this->Mode = CPUMode::Running;
}
