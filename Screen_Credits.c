#include "System_Input.h"
#include "System_Graphics.h"
#include "System_MessageWindow.h"
#include "Common.h"
#include <c64.h>
#include <stdio.h>
#include <conio.h>

byte yscroll;
int xoffset = 0;
int xoffsetdir = 1;

void ScrollUp()
{
  byte i = 0;  
  yscroll--;
  VIC.ctrl1 = VIC.ctrl1 & 0xf8;
  VIC.ctrl1 |= (yscroll & 7);   
  if ((yscroll & 7) == 7)
  {
    MoveScreenUp();
    gotoxy(0, 24);
    PrintString("Hello@", xoffset, 24, true, false);
    xoffset += xoffsetdir;
    if (xoffset < 1)
      xoffsetdir = 1;
    if (xoffset > 8)
      xoffsetdir = -1;
  }
  wait_vblank(7);
}

screenName Update_Credits()
{
  screenName nextScreen = Title;
  bool exit = false;
  yscroll = 0;
  
  ClearScreen();
 // WriteLineMessageWindow("Credits@", 0);
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exit = true;
    ScrollUp();
    
  }
  
  return nextScreen;
}