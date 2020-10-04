#pragma once
#include "System/Printer.h"
#include "System/ButtonSystem.h"
#include "MenuHandler.h"

enum class MainMenuMode : uint8_t
{
	GameList, Settings, Credits, Load, 
};

class MainMenu
{
private:
//layout
  static constexpr uint8_t yStart = 8;
  static constexpr uint8_t lines = 4;
  static constexpr uint8_t scrollMargin = 2;
  int8_t scroll = 0;   //first drawn line
//Contents
	MainMenuMode mode = MainMenuMode::GameList;
  MenuHandler menu;
  
  void drawGameList(Screen64x32& screen, Printer& printer)
  {
	const uint8_t selected = menu.getSelected();
    const uint8_t first = scroll;
    const uint8_t last = min(first + lines, menu.getSize());

    //selection box. Kinda hacky, but this is the look I wanna go for
    const uint8_t selectedOffset = (this->yStart + (selected - first) * 6) - 1;
    for(uint8_t y = selectedOffset; y < selectedOffset + 7; ++y)
    {
      for(uint8_t i = 0; i < 64; ++i)
        screen.setPixel(i, y, 1);
    }

    printer.setPosition(1, this->yStart);
    printer.setXStart(1);
    for(uint8_t i = first; i < last; ++i)
    {
      printer.setInverted((i == selected));
      printer.print(F("NAMECHR"));
      printer.println(i);
    }
    printer.setInverted(false);
  }
  void drawCredits(Screen64x32& screen, Printer& printer)
  {
	  printer.setPosition(0,8);
	  printer.print(F("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?"));
  }
public:
	bool loadGame()
	{
		return (this->mode == MainMenuMode::Load);
	}

  void update(ButtonSystem& buttons)
  {
	//Selection
    const uint8_t menuSize = menu.getSize();
    uint8_t selected = menu.getSelected();
    if(buttons.justPressed(UP_BUTTON))
      selected = (selected > 0) ? selected - 1 : 0;
    if(buttons.justPressed(DOWN_BUTTON))
      selected = (selected < (menuSize - 1)) ? selected + 1 : menuSize - 1;
    menu.setSelected(selected);
	
	if(buttons.justPressed(LEFT_BUTTON))
		this->mode = MainMenuMode::Credits;
	if(buttons.justPressed(RIGHT_BUTTON))
		this->mode = MainMenuMode::GameList;
	
	//Do thing
	if(buttons.justPressed(A_BUTTON))
	{
		this->mode = MainMenuMode::Load;
	}
	
    //scroll menu
    if(selected <= scrollMargin)
      scroll = 0;
    else if(selected > (menuSize - scrollMargin))
      scroll = menuSize - lines;
    else
      scroll = selected - scrollMargin;
  }
  void render(Screen64x32& screen, Printer& printer)
  {
	switch(this->mode)
	{
		case MainMenuMode::GameList:
			this->drawGameList(screen, printer);
		break;
		case MainMenuMode::Credits:
			this->drawCredits(screen, printer);
		break;
	}
  }
};
