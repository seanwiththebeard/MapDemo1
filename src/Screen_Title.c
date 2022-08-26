#include "Common.h"
#include "Platform.h"

screenName Update_Title()
{
  screenName nextScreen = EditParty;
  bool exit = false;
  DrawBorder(12, 6, 16, 5, true, true);
  PrintString("Title Screen@", 14, 8, true, false);
  DrawBorder(12, 20, 16, 5, true, true);
  PrintString("Press Space@", 14, 22, true, false);
  
  //PlaySID();
      
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exit = true;
  }
  
  return nextScreen;
}