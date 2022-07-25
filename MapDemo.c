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

//#resource "song.sid"
//#resource "CharacterSet.bin"
//#resource "c64_1.cfg"
#define CFGFILE c64_1.cfg

#include <conio.h>
#include <c64.h>
#include <stdio.h>

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
  ScreenDisable();
  SelectVICBanks(3, 2, 0);
  ClearScreen();
  bgcolor(0);
  bordercolor(0);
  InitializeInput();
  LoadMap();
  //DiskLoad("CharacterSet.bin", (int)CharRam);
  //DiskLoad("song.sid", (int)SIDLOAD);
  ScreenEnable();
  
}

void main(void)
{  
  //SetCharacterSet();
  
  Initialize();
  SwitchScreen(Title);  
  
  /*while(true)
  {
    UpdateInput();
    Graphics_Update();
    
    if(InputChanged())
      UpdateScreen();
  }*/
}