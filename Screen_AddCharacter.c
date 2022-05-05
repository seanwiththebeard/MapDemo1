#include "Common.h"
#include "System_Graphics.h"
#include "System_Input.h"
#include <string.h>

byte windowX = 1;
byte windowY = 1;
#define windowWidth 16
#define windowHeight 12

#define DrawSelection() (SetChar(windowX + 2, windowY + selection + 2, '@'))

byte selection = 0;
bool exitWindow = false;
bool nextWindow = false;


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
  selection = 0;
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
      switch (selection)
      {
        case 4:
          exitWindow = true;
          break;
        default:
          nextWindow = true;
          windowX += 4;
          windowY += 4;
          break;
      }
    }
}



void GetClass()
{
  nextWindow = false;
  exitWindow = false;
  DrawCharWindow(windowX, windowY, windowWidth, windowHeight, "Class?@");
  while (!nextWindow)
  {
    UpdateInput();
    if (InputChanged())
      WindowInput();
    if (exitWindow)
    return;
  }
}

void GetRace()
{
  nextWindow = false;
  exitWindow = false;
  DrawCharWindow(windowX, windowY, windowWidth, windowHeight, "Race?@");
  while (!nextWindow)
  {
    UpdateInput();
    if (InputChanged())
      WindowInput();
    if (exitWindow)
    return;
  }
  if (nextWindow)
    GetClass();
}

void GetStats()
{
  windowX = 1;
  windowY = 1;
  exitWindow = false;
  nextWindow = false;
  DrawCharWindow(windowX, windowY, windowWidth, windowHeight, "Stats?@");
  while (!nextWindow)
  {
    UpdateInput();
    if (InputChanged())
      WindowInput();
    if (exitWindow)
    return;
  }
  
  exitWindow = false;
  if (nextWindow)
    GetRace();
}

void DrawAddCharacterScreen()
{
  GetStats();
  CopyDoubleBuffer();
  //CopyDoubleBufferArea(windowX, windowY, windowWidth, windowHeight);  
}