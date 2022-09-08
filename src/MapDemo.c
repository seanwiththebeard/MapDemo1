#include "Platform.h"
//#link "Common.c"
//#link "Screen_Map.c"
//#link "System_CharacterSets.c"
//#link "System_MessageWindow.c"
//#link "Screen_Title.c"
//#link "Screen_AddCharacter.c"
//#link "BFRPG.c"
//#link "Lists.c"
//#link "Screen_Combat.c"
//#link "Screen_Credits.c"
//#link "SIDPlayerASM.s"
//#link "CharacterSetASM.s"

//#link "System_Graphics.c"
//#link "System_Audio.c"
//#link "System_FileIO.c"
//#link "System_Input.c"


#include <conio.h>

#include "Common.h"
#include "Screen_Map.h"
#include "Screen_Title.h"
#include "Screen_AddCharacter.h"
#include "System_CharacterSets.h"
#include "System_MessageWindow.h"
#include "BFRPG.h"
#include "Lists.h"
#include "Screen_Combat.h"
#include "Screen_Credits.h"


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

void apple (void)
{
  byte x, y;
  byte i = 0;

  ClearScreen();
  
  while (1)
  {
    for (y= 0; y < 16; ++y)
      for (x = 0; x < 16; ++x)
      {
        //DrawChar(i, x, y);
        ++i;
        //DrawChar(x + y*16, x + (i % 24), y + (i % 8)); 
      }
    //FlipBuffer();
    i += 16;
  }
}

void main(void)
{
  Initialize();
  //apple();
  SwitchScreen(Map);  
  ClearScreen();
  while(1)
  {}
}