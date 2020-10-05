#pragma once
#include "System/Size.h"

#include "Games/15PUZZLE.h"
#include "Games/BLINKY.h"
#include "Games/BLITZ.h"
#include "Games/BRIX.h"
#include "Games/CONNECT4.h"
#include "Games/GUESS.h"
#include "Games/HIDDEN.h"
#include "Games/IBM.h"
#include "Games/INVADERS.h"
#include "Games/KALEID.h"
#include "Games/MAZE.h"
#include "Games/MERLIN.h"
#include "Games/MISSILE.h"
#include "Games/PONG.h"
#include "Games/PONG2.h"
#include "Games/PUZZLE.h"
#include "Games/SYZYGY.h"
#include "Games/TANK.h"
#include "Games/TESTOPCD.h"
#include "Games/TETRIS.h"
#include "Games/TETRIS.h"
#include "Games/TICTAC.h"
#include "Games/UFO.h"
#include "Games/VBRIX.h"
#include "Games/VERS.h"
#include "Games/WIPEOFF.h"

struct GameEntry
{
  const char* Name;
  const uint8_t* Data;
  size_t Size;

  template<size_t size>
  constexpr GameEntry(const char * name, const uint8_t (&data)[size]) :
    Name { name }, Data { data }, Size { size }
  {
  }
};

constexpr GameEntry GameList[] PROGMEM =
{
  { FifteenPuzzleName, FifteenPuzzleData },
  { BlinkyName, BlinkyData },
  { BlitzName, BlitzData },
  { BrixName, BrixData },
  { Connect4Name, Connect4Data },
  { GuessName, GuessData },
  { HiddenName, HiddenData },
  { IBMName, IBMData },
  { InvadersName, InvadersData },
  { KaleidName, KaleidData },
  { MazeName, MazeData },
  { MerlinName, MerlinData },
  { MissileName, MissileData },
  { PongName, PongData },
  { Pong2Name, Pong2Data },
  { PuzzleName, PuzzleData },
  { SyzygyName, SyzygyData },
  { TankName, TankData },
  { TestOPCodeName, TestOPCodeData },
  { TetrisName, TetrisData },
  { TictacName, TictacData },
  { UFOName, UFOData },
  { VBrixName, VBrixData },
  { VersName, VersData },
  { WipeoffName, WipeoffData },
};
