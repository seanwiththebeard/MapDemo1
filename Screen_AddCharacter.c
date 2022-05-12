#include "Common.h"
#include "System_Graphics.h"
#include "System_Input.h"
#include "System_MessageWindow.h"
#include "BFRPG.h"
#include "Lists.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void GetRace(void);

byte windowX = 2;
byte windowY;
byte windowWidth = 16;
byte windowHeight = 12;

char Selections[8][16];
byte selection = 0;
byte countSelections = 4;
bool exitWindow = false;
bool nextWindow = false;

byte WindowLevel = 0;

byte CurrentCharacter = 0;

byte
    	HPMAX,
	HP,
  	STR,
  	DEX,
        CON,
        INT,
  	WIS,
	CHR,
	
	RACE,
	CLASS,
	HITDICE;

void AddToParty()
{
  struct playerChar *PlayerChar;
  create();
  PlayerChar = getPlayerChar(CountRoster() - 1);
  PlayerChar->HPMAX = HPMAX;
  PlayerChar->HP = HP;
  PlayerChar->STR = STR;
  PlayerChar->DEX = DEX;
  PlayerChar->CON = CON;
  PlayerChar->INT = INT;
  PlayerChar->WIS = WIS;
  PlayerChar->CHR = CHR;
  PlayerChar->RACE = RACE;
  PlayerChar->CLASS = CLASS;
  sprintf(PlayerChar->NAME, "Hello %d @", CountRoster() - 1);
  
  if (CountRoster() <= 4)
    DrawCharStats(CountRoster() - 1);
  exitWindow = true;
  nextWindow = true;
}

byte RollDice(byte count, byte diceSize)
{
  byte result = 0;
  byte i;
  for (i = 0; i < count; i++)
    result += (rand() %diceSize + 1);
  return result;
}

#define DrawSelection() (SetChar(windowX + 2, windowY + selection + 1, '>'))

void SetString(char value[16], byte menuItem)
{
  byte x;
  for (x = 0; x < 16; ++x)
    Selections[menuItem][x] = value[x];
    
}

void MoveSelection(bool direction)
{
  SetChar(windowX + 2, windowY + selection + 1, ' ');
  
  if (!direction && selection > 0)
    --selection;
  
  if (direction && selection < countSelections)
    ++selection;
  
  DrawSelection();
}

void DrawSelections()
{
  byte x;
  for (x = 0; x < countSelections + 1; ++x)
  {
    PrintString(Selections[x], windowX + 3, windowY + 1 + x, false, false);
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
  
  DrawBorder(xPos, yPos, width, height, false);
  
  PrintString(title, xPos + 1, yPos, false, false);
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
  
  SetString("+ Fighter@", 0);
  SetString("+ Magic-User@", 1);
  SetString("+ Cleric@", 2);
  SetString("+ Thief@", 3);
  SetString("Exit@", 4);  
  
  if (STR < 9)
    Selections[0][0] = ' ';
  if (INT < 9)
    Selections[1][0] = ' ';
  if (WIS < 9)
    Selections[2][0] = ' ';
  if (DEX < 9)
    Selections[3][0] = ' ';    
  
  //sprintf(str, "Seed: %d@", randseed);
  //WriteLineMessageWindow(str, 0);  
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
      if (WindowLevel == 2)
      {
        RollStats();
        DrawSelections();
      }
    }
    if (InputFire())
    {
      if (WindowLevel == 0)
      	if (selection == 1)
          if (CountRoster() < 12)
            GetRace();
      
      if (WindowLevel == 2)
        if (Selections[selection][0] == ' ')
        {
          WriteLineMessageWindow("Prime stat low@", 0);
          return;
        }
      if (selection == countSelections)
      {
        exitWindow = true;
      }
      else if (Selections[selection][0] != ' ')
        nextWindow = true;
    }
}

void GetClass()
{
  int temp = 0;
  CLASS = 0;
  WindowLevel = 2;
  windowY = 10;
  windowHeight = 7;
  countSelections = 4;
  
  nextWindow = false;
  exitWindow = false;
    
  RollStats();
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
    CLASS = selection;
    WriteLineMessageWindow(ClassDescription[CLASS].NAME, 0);
    
    if (RaceDescription[RACE].HITDICEMAX < ClassDescription[CLASS].HITDICE)
      HITDICE = RaceDescription[RACE].HITDICEMAX;
    else
      HITDICE = ClassDescription[CLASS].HITDICE;
    
    temp = RollDice(1, HITDICE);
    sprintf(str, "Hit Dice: 1d%d@", HITDICE);
    WriteLineMessageWindow(str, 0);
    sprintf(str, "Roll: %d + MOD %d@", temp, AbilityModifier[CON]);
    WriteLineMessageWindow(str, 0);
    if (temp + AbilityModifier[CON] < 1)
    {
      WriteLineMessageWindow("Died, no HP@", 0);
      exitWindow = true;
      nextWindow = true;
    }
    else
    {
      HPMAX = temp + AbilityModifier[CON];
      HP = HPMAX;
      AddToParty();
    }
  }
  else
    GetClass();
}

void GetRace()
{
  RACE = 0;
  WindowLevel = 1;
  windowY = 5;
  windowHeight = 7;
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
    RACE = selection;
    GetClass();
  }
}

void GetStats()
{
  WindowLevel = 0;
  windowY = 3;
  windowHeight = 4;
  countSelections = 1;
  SetString("Confirm@", 0);
  SetString("Exit@", 1);
  
  exitWindow = false;
  nextWindow = false;
  
  DrawCharWindow(windowX, windowY, windowWidth, windowHeight, "Stats?@");
  //RollStats();
  
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
    nextWindow = false;
  }
}

void DrawRoster()
{
  byte temp = 0;
  struct playerChar *PlayerChar = getPlayerChar(temp);
  
  sprintf(str, "Count: %d@", CountRoster());
    WriteLineMessageWindow(str, 0);
  
  countSelections = 3;
  WindowLevel = 0;
  windowX = 0;
  windowY = 0;
  windowWidth = COLS - 2;
  windowHeight = ROWS - 2;
  exitWindow = false;
  nextWindow = false;
  
  SetString("Add to party@", 0);
  SetString("Create@", 1);
  SetString("Delete@", 2);
  SetString("Exit@", 3);
  
  DrawCharWindow(windowX, windowY, COLS - 2, ROWS - 2, "Roster@");
  
  
  for (temp = 0; temp < CountRoster(); ++temp)
  {
    sprintf(str, "%s@", PlayerChar->NAME);
    PrintString(str, windowX + 3, windowY + 8 + temp, false, false);
    sprintf(str, "%s@", RaceDescription[PlayerChar->RACE].NAME);
    PrintString(str, windowX + 12, windowY + 8 + temp, false, false);
    sprintf(str, "%s@", ClassDescription[PlayerChar->CLASS].NAME);
    PrintString(str, windowX + 20, windowY + 8 + temp, false, false);
    PlayerChar = PlayerChar->next;
  }
  
  while(!exitWindow)
  {
    UpdateInput();
    if (InputChanged())
      WindowInput();
  }
  CopyDoubleBuffer();
}

void DrawAddCharacterScreen()
{
  exitWindow = false;
  CurrentCharacter = 0;
  
  DrawRoster();
  
  while (CurrentCharacter < 4 && !exitWindow)
  {
    WriteLineMessageWindow("@", 0);
    WriteLineMessageWindow("New Character@", 0);
    GetRace();
    CopyDoubleBuffer();
    ++CurrentCharacter;
  }
  //CopyDoubleBufferArea(windowX, windowY, windowWidth, windowHeight);  
}

