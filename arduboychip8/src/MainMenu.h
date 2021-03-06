#pragma once
#include "System/FlashStringHelper.h"
#include "System/Printer.h"
#include "System/ButtonSystem.h"
#include "MenuHandler.h"
#include "System/Size.h"
#include "GameList.h"

enum class MainMenuMode : uint8_t
{
	GameList, Settings, Credits, Load,
};

constexpr MainMenuMode firstMenuPage = MainMenuMode::GameList;
constexpr MainMenuMode lastMenuPage = MainMenuMode::Credits;

class MainMenu
{
private:
//layout
	static constexpr uint8_t yStart = 8;
	static constexpr uint8_t lines = 4;
	static constexpr uint8_t scrollMargin = 2;
	uint8_t scroll = 0; //first drawn line
//Contents
	MainMenuMode mode = MainMenuMode::GameList;
	MenuHandler menuGamelist = MenuHandler(getSize(GameList));

	void scrollMenu(MenuHandler& menu)
	{
		const uint8_t selected = menu.getSelected();
		const uint8_t menuSize = menu.getSize();
		if(selected <= scrollMargin)
			scroll = 0;
		else if(selected > (menuSize - scrollMargin))
			scroll = menuSize - lines;
		else
			scroll = selected - scrollMargin;
	}

	void tickGameList(ButtonSystem& buttons)
	{
		if(buttons.justPressed(UP_BUTTON))
			menuGamelist.selectPrevious();
		if(buttons.justPressed(DOWN_BUTTON))
			menuGamelist.selectNext();

		//Load selected game thing
		if(buttons.justPressed(A_BUTTON))
		{
			this->mode = MainMenuMode::Load;
		}

		this->scrollMenu(menuGamelist);
	}
	void drawGameList(Screen64x32& screen, Printer& printer)
	{
		const uint8_t selected = menuGamelist.getSelected();
		const uint8_t first = scroll;
		const uint8_t last = min(first + lines, menuGamelist.getSize());

		// Draw the selection box
		const uint8_t selectedOffset = (this->yStart + (selected - first) * 6) - 1;
		screen.fillRectangle(0, selectedOffset, screen.width, 7, ~0);

		printer.setPosition(1, this->yStart);
		printer.setXStart(1);

		for(uint8_t i = first; i < last; ++i)
		{
			if(i == selected)
				printer.setInverted(true);

			printer.print(AsFlashStringHelper(pgm_read_ptr(&GameList[i].Name)));
			printer.println(i);

			if(i == selected)
				printer.setInverted(false);
		}
	}

	void drawCredits(Screen64x32& screen, Printer& printer)
	{
		printer.setPosition(0,yStart);
		printer.print(F("Credits\n @Spacebruce\n Pharap"));
	}
	void tickSettings(ButtonSystem& buttons)
	{

	}
	void drawSettings(Screen64x32& screen, Printer& printer)
	{

	}
public:
	size_t gameSelected() const
	{
		return this->menuGamelist.getSelected();
	}

	bool loadGame()
	{
		return (this->mode == MainMenuMode::Load);
	}

	void update(ButtonSystem& buttons)
	{
		///Change menu
		if(buttons.justPressed(LEFT_BUTTON))
		{
			this->mode = (this->mode == firstMenuPage) ? lastMenuPage :
				static_cast<MainMenuMode>((static_cast<uint8_t>(this->mode) - 1));
		}
		if(buttons.justPressed(RIGHT_BUTTON))
		{
			this->mode = (this->mode == lastMenuPage) ? firstMenuPage :
				static_cast<MainMenuMode>((static_cast<uint8_t>(this->mode) + 1));
		}

		switch(this->mode)
		{
			case MainMenuMode::GameList:
				tickGameList(buttons);
			break;
			case MainMenuMode::Settings:
				tickSettings(buttons);
			break;
		}
	}
	void render(Screen64x32& screen, Printer& printer)
	{
	switch(this->mode)
	{
		case MainMenuMode::GameList:
			this->drawGameList(screen, printer);
		break;
		case MainMenuMode::Settings:

		break;
		case MainMenuMode::Credits:
			this->drawCredits(screen, printer);
		break;
	}
	}
};
