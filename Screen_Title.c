#include "System_Input.h"
#include "System_Graphics.h"
#include "System_CharacterSets.h"
#include "Common.h"

byte TitleCursorX = 0;
byte TitleCursorY = 0;
byte TitlePosX = 23;
byte TitlePosY = 1;
byte color = 1;
byte index = 22;


void Draw_Title()
{
   }
void Update_Title()
{
    if (InputDown() && TitleCursorY < 25)
        TitleCursorY++;
    if (InputUp() && TitleCursorY > 0)
        TitleCursorY--;
    Draw_Title();
}

void Init_Title()
{
    Draw_Title();
}