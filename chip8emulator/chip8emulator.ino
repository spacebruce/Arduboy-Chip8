#include <Arduboy2.h>
#include <arduino.h>
#include "testrom.h"
#include "chip8.h"
#include "debugmodule.h"
#include "config.h"

Arduboy2 System;

Chip8 Chip8;
DebugModule Debug = DebugModule(&Chip8);

void setup()
{
  System.begin();
  System.clear();
  System.setFrameRate(60);
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif
  Chip8.Load(TestRom);
}

void loop()
{
  if (!(System.nextFrame()))
    return;
  System.pollButtons();

  if(System.justPressed(A_BUTTON))
    Chip8.Tick(&System, 1);
  if(System.pressed(B_BUTTON))
    Chip8.Tick(&System);

  Debug.Tick(&System);
  Debug.Draw(&System);
  System.display();
}
