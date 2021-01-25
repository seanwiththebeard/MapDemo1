#include <stdio.h>
#include <conio.h>
#include <c64.h>

#include "common.h"

#include "c64_bitmapmode.h"
//#link "c64_bitmapmode.c"

#include "map.h"
//#link "map.c"

#include "input.h"
//#link "input.c"

void main(void)
{ 
  InitializeInput();
  clrscr();  
  
  setcolortextmode();
  
  InitializeMapData();
  
  DrawMap();
  
  while(1)
  {
    
    UpdateInput();
    if(InputChanged())
    {
      CheckInput();
      DrawMap();
    }
  }
}