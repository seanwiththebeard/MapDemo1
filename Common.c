#include <peekpoke.h>
#include "Common.h"
#include <c64.h>
#include "System_Graphics.h"

#include "Screen_Title.h"
#include "Screen_Map.h"
#include "Screen_AddCharacter.h"
#include "System_Input.h"

char str[16];
int randseed = 0;
screenName currentScreen = Title;

void SwitchScreen(screenName screen)
{
  ScreenDisable();
  ClearScreen();
  //Unload current screen
  switch (currentScreen)
  {
    case Title:
      break;
    case EditParty:
      break;
    case Map:
      break;
    case Combat:
      break;
    case Menu:
      break;
    default:
      break;
  }
  
  //Load specified screen
  UpdateInput();
  currentScreen = screen;
  ScreenEnable();
  
  switch (currentScreen)
  {
    case Title:
      
      currentScreen = Update_Title();
      break;
    case EditParty:
      currentScreen = DrawAddCharacterScreen();
      break;
    case Map:
      //LoadMap();
      currentScreen = MapUpdate();
      break;
    case Combat:
      break;
    case Menu:
      break;
    default:
      break;
  }
  
  SwitchScreen(currentScreen);
}

void UpdateScreen()
{
  switch (currentScreen)
  {
    case Title:
      break;
    case EditParty:
      break;
    case Map:
      MapUpdate();
      break;
    case Combat:
      break;
    case Menu:
      break;
    default:
      break;
  }
}

byte ReadBit(byte byteToRead, char bit)
{
    bit = 1 << bit;
    return(bit & byteToRead);
}
void WriteBit(byte *byteToSet, char bit, bool value)
{
  if (value)
  {
    bit = 1 << bit;
  }
  else
  {
    bit = 0 << bit;
  }
  *byteToSet = *byteToSet | bit;
}

bool CheckBit(byte source, byte position)
{
  return (source & (1<<position));
}

bool raster_check(byte line)
{
  if(VIC.rasterline == line)
     return true;
     else
     return false;
}