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
  static constexpr uint8_t yStart = 8;
  static constexpr lines = 4;
//Contents

public:
  Menu() = default;

  void render(Printer& printer)
  {
    printer.setPosition(8, yStart);
    printer.setStartX(8);
    for(uint8_t i = 0; i < lines; ++i)
    {
      printer.println(F("NAME8CHR"));
    }
  }
};
