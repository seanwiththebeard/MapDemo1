#define CFGFILE c64_.cfg
//#resource "c64_.cfg"

//#link "common.c"
//#link "Screen_Map.c"
//#link "System_Input.c"
//#link "System_Graphics.c"
//#link "System_StaticScreens.c"
//#link "Data_Graphics.s"
//#link "Data_InitScreen.s"
//#link "Data_Maps.s"

#include "common.h"
#include "Screen_Map.h"
#include "System_Input.h"
#include "System_Graphics.h"
#include "System_StaticScreens.h"


void Initialize()
{
  ScreenDisable();
  SetBackground(0);
  InitializeInput();
  setcolortextmode();
  InitializeMapData();
  SetScreen(0);
  DrawMap();
  ScreenEnable();
}

void main(void)
{
  Initialize();
  
  while(true)
  {
    //for(i = 0;i < 3; i++)
      //raster_wait(255);
    ScrollChar(0,3);
    ScrollChar(0,0);
    
    UpdateInput();
    MapUpdate();
    if(InputChanged())
    {
      CheckInput();
      DrawMap();
    }
    //FlashColor(6, 2);
    
  }
}