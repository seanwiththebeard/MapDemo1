#include "Common.h"
#include "System_Graphics.h"
#include "System_Input.h"

screenName Update_Combat()
{
  screenName nextScreen = Map;
  bool exit = false;
  
  ClearScreen();
  PrintString("Combat!@", 0, 0, true, false);
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exit = true;
  }
  
  return nextScreen;
}