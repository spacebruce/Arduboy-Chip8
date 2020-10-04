#pragma once
#include "System/Printer.h"

class MenuEntry
{
private:
public:
};

class Menu
{
private:
//layout
  uint8_t y = 8;
  uint8_t lines = 4;
//Contents
public:
  Menu() = default;

  void render(Printer& printer)
  {
    printer.setPosition(0, y);
    for(uint8_t i = 0; i < lines; ++i)
    {
      printer.println(F("name entry!"));
    }
  }
};
