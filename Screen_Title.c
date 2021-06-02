#include "System_StaticScreens.h"
#include "System_Input.h"
#include "System_Graphics.h"
#include "common.h"

byte TitleCursorX = 0;
byte TitleCursorY = 0;
byte TitlePosX = 22;
byte TitlePosY = 6;
byte color = 1;
byte index = 22;


void Draw_Title()
{
    for (TitleCursorY = 0; TitleCursorY < 16; TitleCursorY++)
        for (TitleCursorX = 0; TitleCursorX < 16; TitleCursorX++)
                SetScreenChar(TitleCursorY*16 + TitleCursorX, color, TitleCursorX + TitlePosX, TitleCursorY + TitlePosY);
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
    //SetScreen(1);
    Draw_Title();
}