#include <Arduboy2.h>
#include <arduino.h>
#include "testrom.h"
#include "Chip8Emulator.h"
#include "debugmodule.h"
#include "config.h"

Arduboy2 System;

Chip8Emulator Chip8;
#if DEBUG_MODE
  DebugModule Debug = DebugModule(Chip8);
#endif

void setup()
{
  System.begin();
  System.clear();
  System.setFrameRate(60);

  BeepPin1::begin();

  Chip8.Load(TestRom);
}

void loop()
{
  if (!(System.nextFrame()))
    return;
  System.pollButtons();

  #if DEBUG_MODE
    Debug.PreTick();
  #endif

  Chip8.UpdateDelayTimer();
  Chip8.UpdateSoundTimer();
  Chip8.UpdateInput();

  //if(System.justPressed(A_BUTTON))
  Chip8.Tick(System);
  //if(System.pressed(B_BUTTON))
  //  Chip8.Tick(System);

  #if DEBUG_MODE
    Debug.Tick(System);
    Debug.Draw(System);
  #endif
  System.display();
}
