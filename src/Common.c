#include "Common.h"
#if __C64__
#include <c64.h>
#endif
#include "GameScreens.h"

#include "Platform.h"

char str[16];
int randseed = 0;
screenName currentScreen = Title;

void raster_wait(byte line)
{
  #if __C64__
  while ((VIC.rasterline < line))
  {}
  #endif
}

void wait_vblank(byte frames) 
{
  byte count = frames;
  for (count = frames; count; --count)
  {
    raster_wait(255);    
  }
}

void SwitchScreen(screenName screen)
{
  ScreenDisable();
  ClearScreen();
  //Load specified screen
  UpdateInput();
  currentScreen = screen;
  ScreenEnable();
  
  switch (currentScreen)
  {
    case Title:
      currentScreen = Update_Title();
      break;
    case Credits:
      currentScreen = Update_Credits();
      break;
    case EditParty:
      currentScreen = DrawAddCharacterScreen();
      break;
    case Map:
      //LoadMap();
      currentScreen = MapUpdate();
      break;
    case Combat:
      currentScreen = Update_Combat();
      break;
    case Menu:
      break;
    default:
      break;
  }
  
  SwitchScreen(currentScreen);
}

byte ReadBit(byte byteToRead, char bit)//These are old
{
    bit = 1 << bit;
    return(bit & byteToRead);
}
void WriteBit(byte *byteToSet, char bit, bool value)//These are old
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

byte setBit(byte byteToSet , byte k) //These work right
{
    byteToSet = byteToSet | (1 << (k - 1));
}
byte clearBit(byte byteToSet , byte k) //These work right
{
    byteToSet = byteToSet & (~(1 << (k)));
}

bool CheckBit(byte source, byte position) //These are old
{
  return (source & (1<<position));
}

bool raster_check(byte line)
{
  #if __C64__
  if(VIC.rasterline == line)
     return true;
     else
     return false;
  #endif
}