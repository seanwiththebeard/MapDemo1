//#include <stdio.h>
#include <conio.h>  //For clrscr

//#link "map.c"
//#link "input.c"
//#link "c64_bitmapmode.c"
//#link "bitwiseops.c"

#include "map.h"
#include "input.h"
#include "common.h"
#include "bitwiseops.h"
#include "c64_bitmapmode.h"


void Initialize()
{
  InitializeInput();
  clrscr();
  
  setcolortextmode();
  
  InitializeMapData();
  
  DrawMap();
}

void main(void)
{ 
  Initialize();
  
  while(true)
  {
    //MapUpdate();
    //DrawMap();
    //wait_vblank();
    ScrollChar(0);
    
    UpdateInput();
    if(InputChanged())
    {
      CheckInput();
      DrawMap();
    }
  }
}