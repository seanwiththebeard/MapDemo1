#include "System_MessageWindow.h"
#include "Common.h"
#include <stdio.h>
#include "Platform.h"

byte yscroll;
/*byte sinevalue[32] = {12,14,17,19,20,22,23,24,
24,24,23,22,20,19,17,14,
12,10,7,5,4,2,1,0,
0,0,1,2,4,5,7,10};*/
byte xcredit;
byte delay;
byte xoff;
bool skipline;

#define linecount 14

char CreditsLines[][20] =
{
  "Credits@",
  "(c) 2022@",
  "All rights reserved@",
  
  "Lead Designer@",
  "and Programmer:@",
  "Somebody@",
  
  "Makes use of the@",
  "following@",
  "libraries@",
  
  "C/ASM SID Player:@",
  "https://github.com/@",
  "solidcore-commodore@",
  
  "Special Thanks@",
  "YOU!@"
  };

void ScrollUp()
{
  if (xcredit == linecount)
  ++delay;
  
  MoveScreenUp();
  if (xcredit < linecount)
  {
    if (xoff %4 != 0)
    {
      PrintString(CreditsLines[xcredit], xcredit % 3, ROWS - 1, true, true); //Writes to the buffer before we flip the buffer
      ++xcredit;   
    }
    else
    MoveScreenUp();
    //PrintString("@", xcredit % 3, 24, true, true);
  }
  ++xoff;
}

screenName Update_Credits()
{
  screenName nextScreen = Title;
  bool exit = false;
  yscroll = 0;
  xcredit = 0;
  delay = 0;
  xoff = 0;
  skipline = false;
  
  ClearScreen();
  #if __C64__
  ScrollingMaskOn();
  PlaySID();
  #endif
  
  while (!exit)
  {
    UpdateInput();
    ScrollUp();
    
    if (InputChanged())
      if (InputFire())
        exit = true;
    if (delay == 35)
      exit = true;
  }
  #if __C64__
  StopSID();
  ScrollingMaskOff();
  ScrollReset();
  #endif
  return nextScreen;
}