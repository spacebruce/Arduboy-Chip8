#pragma once

// For size_t
#include <stddef.h>

// For uint8_t
#include <stdint.h>

// For PROGMEM
#include <avr/pgmspace.h>

// For getSize
#include "Size.h"

constexpr uint8_t FontData[] PROGMEM
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80, // F
  0xF0, 0x80, 0xB0, 0x90, 0xF0, // G
  0x90, 0x90, 0xF0, 0x90, 0x90, // H
  0x70, 0x20, 0x20, 0x20, 0x70, // I
  0xF0, 0x20, 0x20, 0xA0, 0x60, // J
  0x90, 0xA0, 0xC0, 0xA0, 0x90, // K
  0x80, 0x80, 0x80, 0x80, 0xF0, // L
  0x90, 0xF0, 0x90, 0x90, 0x90, // M (?)
  0x90, 0xD0, 0xF0, 0xB0, 0x90, // N
  0x60, 0x90, 0x90, 0x90, 0x60, // O
  0xE0, 0x90, 0xE0, 0x80, 0x80, // P
  0x60, 0x90, 0x90, 0xA0, 0x50, // Q
  0xE0, 0x90, 0xE0, 0xA0, 0x90, // R
  0x60, 0x80, 0x60, 0x10, 0x60, // S
  0x70, 0x20, 0x20, 0x20, 0x20, // T
  0x90, 0x90, 0x90, 0x90, 0x60, // U
  0x90, 0x90, 0x90, 0x60, 0x60, // V
  0x90, 0x90, 0x90, 0xF0, 0x90, // W (?)
  0x90, 0x90, 0x60, 0x90, 0x90, // X
  0x90, 0x90, 0x60, 0x60, 0x60, // Y
  0xF0, 0x10, 0x20, 0x40, 0xF0, // Z
  0x60, 0x90, 0x20, 0x00, 0x20, // ?
  0x20, 0x20, 0x20, 0x00, 0x20, // !
};

constexpr size_t FontDataSize = getSize(FontData);
