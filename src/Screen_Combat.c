#include "Common.h"
#include "Lists.h"
#include "Platform.h"

byte index = 36;

void DrawMap()
{
  byte x, y = 0;
  SetTileOrigin(1, 1);
  wait_vblank(1);
  for (y = 0; y < 8; ++y)
    for (x = 0; x < 8; ++x)
    {
      DrawTileFast(index, x, y);
    }
}

void DrawCharacters()
{
  byte i;
  for (i = 0; i < CountParty(); ++i)
    DrawTileFast(i, 2+i, 6);
}

screenName Update_Combat()
{
  screenName nextScreen = Map;
  bool exit = false;
  
  ClearScreen();
  PrintString("Combat!@", 0, 0, true, false);
  DrawBorder(0, 0, 18, 18, true, true);
  DrawMap();
  DrawCharacters();
  CopyDoubleBuffer();
  
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exit = true;
  }
  
  return nextScreen;
}