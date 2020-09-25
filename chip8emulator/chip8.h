#pragma once
#include <stdint.h>
#include <stddef.h>
#include <arduino.h>
#include <avr/pgmspace.h>
#include "config.h"

enum class MemoryPartition
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
  uint8_t* Rom;
  size_t RomSize;
  size_t RomStart = 0x200;
  size_t RomEnd = 0;
//CPU/shit
  bool Running = false;
  uint8_t Register[16];
  uint16_t Index;
  uint16_t ProgramCounter = 0;
  uint16_t Stack[16];
  uint16_t StackPointer = 0;
  uint16_t TimerDelay = 0;
  uint16_t TimerSound = 0;
//things
  MemoryPartition GetMemoryPartition(const size_t Location) const;
  void ExecuteInstruction();
  uint8_t ReadMemory(const size_t Location) const;
public:
  Chip8() = default;
  Chip8(uint8_t* Rom, const size_t RomSize);

  void Load(uint8_t* Rom, const size_t RomSize);
  void Reset();
  void Draw();
  void Tick();
};
