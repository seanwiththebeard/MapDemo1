#include "Platform.h"

#include <conio.h> //For border/background color?

#include "Common.h"
#include "GameScreens.h"


void Initialize()
{
  #if __C64__
  ScreenDisable();
  SelectVICBanks(3, 2, 7);
  ClearScreen();
  bgcolor(0);
  bordercolor(0);
  InitializeInput();
  DiskLoad("song.dat", (int)SIDLOAD);
  DiskLoad("CharacterSet.bin", (int)CharRam);
  LoadMap();
  ScreenEnable();
  #endif
}

void main(void)
{
  Initialize();
  SwitchScreen(Map);  
  ClearScreen();
}