#include "Common.h"
#include "System_Graphics.h"
#include "System_Input.h"
#include <string.h>

#define windowX 1
#define windowY 1
#define windowWidth 16
#define windowHeight 12

#define DrawSelection() (SetChar(windowX + 2, windowY + selection + 2, '@'))

byte selection = 0;
bool exitWindow = false;


void MoveSelection(bool direction)
{
  SetChar(windowX + 2, windowY + selection + 2, ' ');
  
  if (!direction && selection > 0)
    --selection;
  
  if (direction && selection < 4)
    ++selection;
  
  DrawSelection();
}

void DrawCharWindow(byte xPos, byte yPos, byte width, byte height, char title[16])
{
  byte x;
  for (x = 0; x < height; ++x)
  {
    DrawLineH(' ', 0, xPos, yPos + x, width);
  }
  
  //char index, byte color, byte x, byte y, byte length
  DrawLineH('0', 1, xPos, yPos, width);
  DrawLineH('0', 1, xPos, yPos + height - 1, width);
  DrawLineV('0', 1, xPos, yPos, height);
  DrawLineV('0', 1, xPos + width - 1, yPos, height);
  PrintString(title, xPos + 1, yPos, false);
  DrawSelection();
}

void WindowInput()
{
  if (InputUp())
  {
    MoveSelection(false);
  }
    if (InputDown()) 
    {
      MoveSelection(true);
    }
    if (InputLeft())
    {
    }
    if (InputRight())
    {
    }
    if (InputFire())
    {
      exitWindow = true;
    }
}

void DrawAddCharacterScreen()
{
  byte i;
  exitWindow = false;
  for (i = 1; i < 2; ++i)
  {
    DrawCharWindow(i, i, windowWidth, windowHeight, "Message@");
    while (!exitWindow)
    {
      UpdateInput();
      if (InputChanged())
        WindowInput();
    }
    CopyDoubleBufferArea(windowX, windowY, windowWidth, windowHeight);
  }
}