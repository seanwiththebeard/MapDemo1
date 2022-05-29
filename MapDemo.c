//#link "Common.c"
//#link "Screen_Map.c"
//#link "System_Input.c"
//#link "System_Graphics.c"
//#link "System_CharacterSets.c"
//#link "System_MessageWindow.c"
//#link "Screen_Title.c"
//#link "Screen_AddCharacter.c"
//#link "System_MusicPlayer.c"
//#link "BFRPG.c"
//#link "Lists.c"
//#link "Screen_Combat.c"
//#link "Screen_Credits.c"


#include <conio.h>
#include <c64.h>

#include "Common.h"
#include "Screen_Map.h"
#include "Screen_Title.h"
#include "Screen_AddCharacter.h"
#include "System_Input.h"
#include "System_Graphics.h"
#include "System_CharacterSets.h"
#include "System_MessageWindow.h"
#include "System_MusicPlayer.h"
#include "BFRPG.h"
#include "Lists.h"
#include "Screen_Combat.h"
#include "Screen_Credits.h"

void Initialize()
{
  ScreenDisable();
  bgcolor(0);
  bordercolor(0);

  InitializeInput();
  setcolortextmode();
  SetCharacterSet();
  //Draw_Title();
  LoadMap();
  
  BlankMessageWindow();
  
  DrawCharacterSet(23, 2);
  
  
  
  //DrawAddCharacterScreen();

  //WriteLineMessageWindow("TheQuickBrownFox", 0);
  //WriteLineMessageWindow("JumpsOverLazyDog", 0);
  //WriteLineMessageWindow("0123456789:;<=>?", 0);
  //WriteLineMessageWindow(" ! #$%&'()*+,-./", 0);
  ScreenEnable();
  
}

void main(void)
{  
  Initialize();
  SwitchScreen(Title);  
  
  while(true)
  {
    UpdateInput();
    Graphics_Update();
    music_update();
    
    if(InputChanged())
      UpdateScreen();
  }
}