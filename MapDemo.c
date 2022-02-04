//#link "Common.c"
//#link "Screen_Map.c"
//#link "System_Input.c"
//#link "System_Graphics.c"
//#link "System_StaticScreens.c"
//#link "System_CharacterSets.c"
//#link "System_MessageWindow.c"
//#link "Screen_Title.c"

#include "Common.h"
#include "Screen_Map.h"
#include "Screen_Title.h"
#include "System_Input.h"
#include "System_Graphics.h"
#include "System_StaticScreens.h"
#include "System_CharacterSets.h"
#include "System_MessageWindow.h"

void Initialize()
{
  ScreenDisable();
  SetBorder(0);
  SetBackground(0);

  InitializeInput();
  //Init_Title();
  setcolortextmode();
  SetCharacterSet(0);
  ClearScreen();
  
  //Draw_Title();

  BlankMessageWindow();
  CopyDoubleBuffer();

  InitializeMapData();
  DrawEntireMap();
  ScreenEnable();

  //WriteLineMessageWindow("TheQuickBrownFox", 1);
  //WriteLineMessageWindow("JumpsOverLazyDog", 1);
  //WriteLineMessageWindow("0123456789:;<=>?", 1);
  //WriteLineMessageWindow(" ! #$%&'()*+,-./", 1);
}

void main(void)
{
  Initialize();
  
  while(true)
  {
    UpdateInput();
    Graphics_Update();
    //MapUpdate();
    //ScrollChar(0, 0);

    if(InputChanged())
    {
      CheckInput();
    }
  }
}