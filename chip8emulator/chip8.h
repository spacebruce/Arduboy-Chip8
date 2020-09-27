#pragma once
#include <stdint.h>
#include <stddef.h>
#include <arduino.h>
#include <Arduboy2.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "font.h"

enum class CPUMode : uint8_t
{
  Startup, Running, Stopped, Error
};
enum class CPUError : uint8_t
{
  None, MemoryWrite, MemoryRead, UnknownOpcode,
};
enum class MemoryPartition : uint8_t
{
  Void, Static, ROM, RAM,
};

class Chip8
{
  friend class DebugModule;
public:
#if SMALL_MEMORY
  static constexpr size_t MemorySize = 1024;
#else
  static constexpr size_t MemorySize = 4096;
#endif
//RAM
  uint8_t Memory[MemorySize];
//ROM
  const uint8_t * Rom;
  size_t RomSize;
  size_t RomStart = 0x200;
  size_t RomEnd = 0;
  static constexpr size_t StackStart = 0x0;
  static constexpr size_t StackSize = 16;
//CPU/shit
  uint8_t Register[16];
  uint16_t RegisterTemp[16];
  uint16_t Index;
  uint8_t StackPointer = StackStart;
  uint16_t Stack[StackSize];
  uint16_t ProgramCounter = 0;
  uint16_t TimerDelay = 0;
  uint16_t TimerSound = 0;
//Debug
  CPUMode Mode = CPUMode::Startup;
  CPUError Error = CPUError::None;
  uint16_t ErrorData = 0;
//things
  MemoryPartition GetMemoryPartition(const size_t Location) const;
  void ExecuteInstruction(Arduboy2 & System);
  uint8_t ReadMemory(const size_t Location);
  void WriteMemory(const size_t Location, const uint8_t Value);
  void PushWord(uint16_t Word);
  uint16_t PullWord();

public:
  Chip8() = default;
  Chip8(const uint8_t * Rom, const size_t RomSize);

  template<size_t size>
  Chip8(const uint8_t (&rom)[size]) :
    Chip8(rom, size)
  {
  }

  void Load(const uint8_t * Rom, const size_t RomSize);
  void Reset(void);
  void Tick(Arduboy2 & System, uint8_t Repeat = 50);
  void Halt(void);

  template<size_t size>
 void Load(const uint8_t (&rom)[size])
  {
    this->Load(rom, size);
  }
};
