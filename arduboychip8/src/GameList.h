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
  const size_t Size;
};

constexpr GameEntry GameList[] PROGMEM =
{
  { FifteenPuzzleName, FifteenPuzzleData, getSize(FifteenPuzzleData) },
  { BlinkyName, BlinkyData, getSize(BlinkyData) },
  { BlitzName, BlitzData, getSize(BlitzData) },
  { BrixName, BrixData, getSize(BrixData) },
  { Connect4Name, Connect4Data, getSize(Connect4Data) },
  { GuessName, GuessData, getSize(GuessData) },
  { HiddenName, HiddenData, getSize(HiddenData) },
  { IBMName, IBMData, getSize(IBMData) },
  { InvadersName, InvadersData, getSize(InvadersData) },
  { KaleidName, KaleidData, getSize(KaleidData) },
  { MazeName, MazeData, getSize(MazeData) },
  { MerlinName, MerlinData, getSize(MerlinData) },
  { MissileName, MissileData, getSize(MissileData) },
  { PongName, PongData, getSize(PongData) },
  { Pong2Name, Pong2Data, getSize(Pong2Data) },
  { PuzzleName, PuzzleData, getSize(PuzzleData) },
  { SyzygyName, SyzygyData, getSize(SyzygyData) },
  { TankName, TankData, getSize(TankData) },
  { TestOPCodeName, TestOPCodeData, getSize(TestOPCodeData) },
  { TetrisName, TetrisData, getSize(TetrisData) },
  { TictacName, TictacData, getSize(TictacData) },
  { UFOName, UFOData, getSize(UFOData) },
  { VBrixName, VBrixData, getSize(VBrixData) },
  { VersName, VersData, getSize(VersData) },
  { WipeoffName, WipeoffData, getSize(WipeoffData) },
};
