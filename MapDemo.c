#include <stdio.h>
#include <conio.h>
#include <c64.h>

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
  
  while(1)
  {
    MapUpdate();
    DrawMap();
    
    UpdateInput();
    if(InputChanged())
    {
      CheckInput();
      DrawMap();
    }
  }
}