//#link "common.c"
//#link "Screen_Map.c"
//#link "System_Input.c"
//#link "System_Graphics.c"
//#link "System_StaticScreens.c"
//#link "System_CharacterSets.c"
//#link "Screen_Title.c"

#include "common.h"
#include "Screen_Map.h"
#include "Screen_Title.h"
#include "System_Input.h"
#include "System_Graphics.h"
#include "System_StaticScreens.h"
#include "System_CharacterSets.h"
#include "System_MessageWindow.h"
#include "cbm_petscii_charmap.h"

void Initialize()
{
  ScreenDisable();
  SetBackground(0);
  SetBorder(0);
  InitializeInput();
  setcolortextmode();
  InitializeMapData();
  SetScreen(0);
  DrawMap();
  SetCharacterSet(0);
  ScreenEnable();

  Init_Title();
  Draw_Title();

  
  BlankMessageWindow();
  
  WriteLineMessageWindow("TheQuickBrownFox");
  WriteLineMessageWindow("JumpsOverLazyDog");
  WriteLineMessageWindow("0123456789:;<=>?");
  WriteLineMessageWindow(" ! #$%&'()*+,-./");

}

void main(void)
{
  Initialize();
  
  while(true)
  {
    UpdateInput();
    Graphics_Update();
    MapUpdate();

    if(InputChanged())
    {
      CheckInput();
      //Update_Title();
      DrawMap();
    }    
  }
}