#include "System_Input.h"
#include "System_Graphics.h"
#include "System_MessageWindow.h"
#include "Common.h"

byte yscroll;
byte sinevalue[32] = {12,14,17,19,20,22,23,24,
24,24,23,22,20,19,17,14,
12,10,7,5,4,2,1,0,
0,0,1,2,4,5,7,10};
byte xoff = 0;
byte xcredit = 0;

char CreditsLines[8][16] = {
    "Credits@",
    "(c) 2022@",
    "Lead Designer:@",
    "Me@",
    "Programmer:@",
    "Somebody@",
    "Special Thanks@",
    "YOU!@"};

void ScrollUp()
{
  byte i = 0;  
  yscroll--;
  VIC.ctrl1 = VIC.ctrl1 & 0xf8;
  VIC.ctrl1 |= (yscroll & 7);
  if ((yscroll & 7) == 7)
  {
    MoveScreenUp();
    ++xoff;
    if (xoff == 31)
      xoff = 0;
    //gotoxy(0, 24);
    if ((xoff & 2) == 2 && xcredit < 8)
    {
      PrintString(CreditsLines[xcredit], xcredit % 2, 24, true, false);
      ++xcredit;
    }
  }
  wait_vblank(4);
}

screenName Update_Credits()
{
  screenName nextScreen = Title;
  bool exit = false;
  yscroll = 0;
  xoff = 0;
  xcredit = 0;
  
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