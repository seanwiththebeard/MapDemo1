//#link "Common.c"
//#link "Screen_Map.c"
//#link "System_Input.c"
//#link "System_Graphics.c"
//#link "System_CharacterSets.c"
//#link "System_MessageWindow.c"
//#link "Screen_Title.c"
//#link "Screen_AddCharacter.c"
//#link "BFRPG.c"
//#link "Lists.c"
//#link "Screen_Combat.c"
//#link "Screen_Credits.c"
//#link "SIDPlayer.c"
//#link "SIDPlayerASM.s"
//#link "FileIO.c"

//#resource "song.sid"

//#resource "c64_1.cfg"
#define CFGFILE c64_1.cfg

#include <conio.h>
#include <c64.h>
#include <stdio.h>

#include "Common.h"
#include "Screen_Map.h"
#include "Screen_Title.h"
#include "Screen_AddCharacter.h"
#include "System_Input.h"
#include "System_Graphics.h"
#include "System_CharacterSets.h"
#include "System_MessageWindow.h"
#include "BFRPG.h"
#include "Lists.h"
#include "Screen_Combat.h"
#include "Screen_Credits.h"
#include "SIDPlayer.h"
#include "FileIO.h"

void Initialize()
{
  DiskSave("charram", (int)CharRam, 0x1000);
  ScreenDisable();
  SelectVICBanks(3, 2, 0);
  //SetCharacterSet();
  ClearScreen();
  bgcolor(0);
  bordercolor(0);
  InitializeInput();
  LoadMap();
  
  BlankMessageWindow();
  
  DrawCharacterSet(23, 2);
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