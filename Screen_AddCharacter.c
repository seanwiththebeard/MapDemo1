#include "Common.h"
#include "System_Graphics.h"
#include <string.h>

void DrawCharWindow(byte xPos, byte yPos, byte width, byte height, char title[16])
{
  byte x;
  for (x = 0; x < height; ++x)
  {
    DrawLineH(' ', 0, xPos, yPos + x, width);
  }
  
  //char index, byte color, byte x, byte y, byte length
  DrawLineH('0', 1, xPos, yPos, width);
  DrawLineH('0', 1, xPos, yPos + height, width);
  DrawLineV('0', 1, xPos, yPos, height);
  DrawLineV('0', 1, xPos + width, yPos, height + 1);
  PrintString(title, xPos + 1, yPos, false);
  CopyDoubleBuffer();
}

void DrawAddCharacterScreen()
{
  DrawCharWindow(4, 4, 16, 9, "Hello@");
}