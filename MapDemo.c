#include <stdio.h>
#include <conio.h>
#include <c64.h>

//#link "map.c"
//#link "input.c"
//#link "c64_bitmapmode.c"

#include "map.h"
#include "input.h"
#include "common.h"
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
    
    UpdateInput();
    if(InputChanged())
    {
      CheckInput();
      DrawMap();
    }
  }
}