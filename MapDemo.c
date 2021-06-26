//#link "common.c"
//#link "Screen_Map.c"
//#link "System_Input.c"
//#link "System_Graphics.c"
//#link "System_StaticScreens.c"
//#link "System_CharacterSets.c"
//#link "System_MessageWindow.c"
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
  SetBorder(1);
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
  
  WriteLineMessageWindow("TheQuickBrownFox", 1);
  WriteLineMessageWindow("JumpsOverLazyDog", 1);
  WriteLineMessageWindow("0123456789:;<=>?", 1);
  WriteLineMessageWindow(" ! #$%&'()*+,-./", 1);

  DrawLineH('0', 7, 22, 0, 17);
  DrawLineH('0', 7, 22, 24, 17);
  DrawLineV('0', 7, 22, 0, 24);
  DrawLineV('0', 7, 39, 0, 24);

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