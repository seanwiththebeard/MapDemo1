#define CFGFILE c64_.cfg
//#resource "c64_.cfg"

#include <conio.h>  //For clrscr

//#link "map.c"
//#link "input.c"
//#link "c64_bitmapmode.c"
//#link "bitwiseops.c"
//#link "common.c"
//#link "charset.ca65"
//#link "mapdata.ca65"
//#link "StaticScreens.c"

#include "map.h"
#include "input.h"
#include "common.h"
#include "bitwiseops.h"
#include "c64_bitmapmode.h"
#include "staticscreens.h"


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
  int i = 0;
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