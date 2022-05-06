#include "Common.h"
#include "System_Graphics.h"
#include "System_Input.h"
#include "System_MessageWindow.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

byte windowX = 1;
byte windowY = 1;
byte windowWidth = 16;
byte windowHeight = 12;

char Selections[8][16];
byte selection = 0;
byte countSelections = 4;
bool exitWindow = false;
bool nextWindow = false;

byte WindowLevel = 0;


byte STR;
byte CON;
byte DEX;
byte WIS;
byte INT;
byte CHR;
byte HP;

byte RollDice(byte count, byte diceSize)
{
  byte result = 1;
  byte i;
  for (i = 0; i < count; i++)
    result += (rand() %diceSize + 1);
  return result;
}

#define DrawSelection() (SetChar(windowX + 2, windowY + selection + 2, '@'))

void SetString(char value[16], byte menuItem)
{
  byte x;
  for (x = 0; x < 16; ++x)
    Selections[menuItem][x] = value[x];
    
}

void MoveSelection(bool direction)
{
  SetChar(windowX + 2, windowY + selection + 2, ' ');
  
  if (!direction && selection > 0)
    --selection;
  
  if (direction && selection < 4)
    ++selection;
  
  DrawSelection();
}

void DrawSelections()
{
  byte x;
  for (x = 0; x < countSelections + 1; ++x)
  {
    PrintString(Selections[x], windowX + 3, windowY + 2 + selection + x, false);
  }
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
  DrawSelections();
}

void RollStats()
{
  srand(randseed);
  STR = RollDice(3, 6);
  CON = RollDice(3, 6);
  DEX = RollDice(3, 6);
  WIS = RollDice(3, 6);
  INT = RollDice(3, 6);
  CHR = RollDice(3, 6);
  
  WriteLineMessageWindow("Rolled Stats:@", 0);
  sprintf(str, "STR: %d CON: %d@", STR, CON);
  WriteLineMessageWindow(str, 0);
  sprintf(str, "DEX: %d WIS: %d@", DEX, WIS);
  WriteLineMessageWindow(str, 0);
  sprintf(str, "INT: %d CHR: %d@", INT, CHR);
  WriteLineMessageWindow(str, 0);
  WriteLineMessageWindow("Right to reroll@", 0);
  
  sprintf(str, "Seed: %d@", randseed);
  WriteLineMessageWindow(str, 0);
  
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
      if (WindowLevel == 0)
        RollStats();
    }
    if (InputFire())
    {
      if (selection == countSelections)
      {
        exitWindow = true;
        WriteLineMessageWindow("Character Tossed@", 0);
      }
      else
        nextWindow = true;
    }
}

void GetClass()
{
  WindowLevel = 2;
  
  windowX = 1;
  windowY = 14;
  windowHeight = 9;
  countSelections = 4;
  SetString("Fighter@", 0);
  SetString("Magic User@", 1);
  SetString("Cleric@", 2);
  SetString("Thief@", 3);
  SetString("Exit@", 4);
  
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
  if (nextWindow)
  {
    WriteLineMessageWindow("Class Confirmed:@", 0);
    WriteLineMessageWindow(Selections[selection], 0);
  }
}

void GetRace()
{
  WindowLevel = 1;
  
  windowX = 1;
  windowY = 6;
  windowHeight = 9;
  countSelections = 4;
  SetString("Human@", 0);
  SetString("Elf@", 1);
  SetString("Dwarf@", 2);
  SetString("Halfling@", 3);
  SetString("Exit@", 4);
  
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
  {
    WriteLineMessageWindow("Race Confirmed:@", 0);
    WriteLineMessageWindow(Selections[selection], 0);
    GetClass();
  }
}

void GetStats()
{
  WindowLevel = 0;
  windowX = 1;
  windowY = 1;
  windowHeight = 6;
  countSelections = 1;
  SetString("Confirm@", 0);
  SetString("Exit@", 1);
  
  exitWindow = false;
  nextWindow = false;
  
  DrawCharWindow(windowX, windowY, windowWidth, windowHeight, "Stats?@");
  RollStats();
  
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
  {
    WriteLineMessageWindow("Stats Confirmed@", 0);
    GetRace();
  }
}

void DrawAddCharacterScreen()
{
  GetStats();
  CopyDoubleBuffer();
  //CopyDoubleBufferArea(windowX, windowY, windowWidth, windowHeight);  
}