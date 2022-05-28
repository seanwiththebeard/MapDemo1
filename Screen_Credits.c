#include "System_Input.h"
#include "System_Graphics.h"
#include "System_MessageWindow.h"
#include "Common.h"
#include <c64.h>

byte yscroll = 0;

void ScrollUp()
{
  byte i;
  byte temp = VIC.ctrl1;
  //WriteLineMessageWindow("@", 0);
  
  for (i = 0; i < 5; ++i)
  {
    wait_vblank(10);
    --VIC.ctrl1;
  }  
  
  //VIC.ctrl1 = temp;
  
  
}

screenName Update_Credits()
{
  
  screenName nextScreen = Title;
  bool exit = false;
  VIC.ctrl1 += 4;
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exit = true;
    WriteLineMessageWindow("Credits@", 20);
    ScrollUp();
    --yscroll;
  }
  
  return nextScreen;
}