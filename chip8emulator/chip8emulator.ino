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
  System.setFrameRate(60);
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif
  Chip8.Load(TestRom, TestRomSize);
}

void loop()
{
  if (!(System.nextFrame()))
    return;
  System.pollButtons();
  //Chip8.Tick();
  //Chip8.Draw();
  Debug.Tick(&System);
  Debug.Draw(&System);

  System.display();
  System.clear();
}
