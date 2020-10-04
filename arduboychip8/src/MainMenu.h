#pragma once
#include "System/Printer.h"
#include "System/ButtonSystem.h"
#include "MenuHandler.h"

class MainMenu
{
private:
//layout
  static constexpr uint8_t yStart = 8;
  static constexpr uint8_t lines = 4;
  int16_t Scroll = 0;
//Contents
  MenuHandler menu;
public:
  MainMenu() = default;

  void update(ButtonSystem& buttons)
  {
    uint8_t selected = menu.getSelected();
    if(buttons.justPressed(UP_BUTTON))
      selected = (selected > 0) ? selected - 1 : 0;
    if(buttons.justPressed(DOWN_BUTTON))
      selected = (selected < (menu.getSize() - 1)) ? selected + 1 : (menu.getSize() - 1);
    menu.setSelected(selected);
    //animate scroll
  }
  void render(Printer& printer)
  {
    printer.setPosition(8, this->yStart);
    printer.setXStart(8);
    for(uint8_t i = 0; i < this->lines; ++i)
    {
      printer.println(F("NAME8CHR"));
    }
    //placeholder
    printer.setPosition(0, this->yStart + (menu.getSelected() * 6));
    printer.print(F("O"));
  }
};
