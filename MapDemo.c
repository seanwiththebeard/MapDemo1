#include "common.h"
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
  SetBackground(0);
  SetBorder(0);

  InitializeInput();
  Init_Title();
  setcolortextmode();
  SetCharacterSet(0);
  ClearScreen();

  Draw_Title();

  InitializeMapData();
  DrawMap(true);
  
  BlankMessageWindow();
  CopyDoubleBuffer();
  ScreenEnable();

  WriteLineMessageWindow("TheQuickBrownFox", 0);
  WriteLineMessageWindow("JumpsOverLazyDog", 0);
  WriteLineMessageWindow("0123456789:;<=>?", 0);
  WriteLineMessageWindow(" ! #$%&'()*+,-./", 0);
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

    //if(InputChanged())
    {
      CheckInput();
    }
  }
}