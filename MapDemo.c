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

void Initialize()
{
  ScreenDisable();
  SetBackground(0);
  InitializeInput();
  setcolortextmode();
  InitializeMapData();
  SetScreen(0);
  DrawMap();
  SetCharacterSet(0);
  ScreenEnable();

  Init_Title();
  Draw_Title();
}

void main(void)
{
  Initialize();
  
  while(true)
  {
    //for(i = 0;i < 3; i++)
      //raster_wait(255);
    //ScrollChar(0,3);
    //ScrollChar(0,0);
    
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