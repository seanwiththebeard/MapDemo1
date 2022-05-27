#include "System_Input.h"
#include "System_Graphics.h"
#include "Common.h"

screenName Update_Title()
{
  screenName nextScreen = EditParty;
  bool exit = false;
  DrawBorder(5, 5, 5, 5, true, true);
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exit = true;
  }
  
  return nextScreen;
}