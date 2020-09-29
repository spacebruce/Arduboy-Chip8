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
  Startup, Running, InputWait, Stopped, Error
};

enum class CPUError : uint8_t
{
  None,
  ExternalWrite,
  ExternalRead,
  UnknownOpcode,
  StackOverflow,
  StackUnderflow,
  AbsentWrite,
  AbsentRead,
  SystemWrite,
  SystemRead,
  RomWrite,
};

enum class MemoryPartition : uint8_t
{
  Void, Static, ROM, RAM,
};

class Chip8Emulator
{
  friend class DebugModule;
public:
#if SMALL_MEMORY
  static constexpr size_t MemorySize = SMALL_MEMORY_ALLOCATED;
#else
  static constexpr size_t MemorySize = 4096;
#endif

  static constexpr uint16_t SoundFrequency = 440;

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
  uint16_t DelayTimer = 0;
  uint16_t SoundTimer = 0;
///Input
  uint8_t InputRegister = 0x0;  //For wait for input instruction
  uint8_t InputPressed = false; //Has user pressed key recently?
  uint8_t InputLast = 0x00;     //Last pressed key id
  bool InputBuffer[16];      //State of all keys
//Debug
  CPUMode Mode = CPUMode::Startup;
  CPUError ErrorType = CPUError::None;
  uint16_t ErrorData = 0;
//things
  MemoryPartition GetMemoryPartition(const size_t Location) const;
  void ExecuteInstruction(Arduboy2 & System);
  uint8_t ReadMemory(const size_t Location);
  void WriteMemory(const size_t Location, const uint8_t Value);
  void PushWord(const uint16_t Word);
  uint16_t PullWord();
  void Error(CPUError ErrorType, uint16_t ErrorData = 0);

public:
  Chip8Emulator() = default;
  Chip8Emulator(const uint8_t * Rom, const size_t RomSize);

  template<size_t size>
  Chip8Emulator(const uint8_t (&rom)[size]) :
    Chip8Emulator(rom, size)
  {
  }

  void Load(const uint8_t * Rom, const size_t RomSize);
  void Reset(void);
  void Tick(Arduboy2 & System);
  void Tick(Arduboy2 & System, uint8_t Repeat);
  void UpdateDelayTimer();
  void UpdateSoundTimer();
  void UpdateInput();
  void Halt(void);
  void SendInput(const uint8_t KeyID);

  template<size_t size>
  void Load(const uint8_t (&rom)[size])
  {
    this->Load(rom, size);
  }
};
