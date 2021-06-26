#include "common.h"
#include "cbm_petscii_charmap.h"
#include "System_Graphics.h"

byte PosX = 22;
byte PosY = 12;
byte Height = 12;
byte Width = 16;
byte Color = 1;
char MessageWindow[16][12];
void DrawMessageWindow()
{
    byte x, y;
    for (y = 0; y < Height; y++)
        for (x = 0; x < Width; x++)
            SetScreenChar(MessageWindow[x][y], Color, PosX + x, PosY + y);
}
void BlankMessageWindow()
{
    byte x, y;
    for (y = 0; y < Height; y++)
        for (x = 0; x < Width; x++)
            MessageWindow[x][y] = ' ';
    DrawMessageWindow();
}

void ScrollMessageWindowUp(byte lines)
{
  byte linecount;
  for (linecount = 0; linecount < lines; linecount++)
  {
    byte x, y;
    for (y = 1; y < Height; y++)
        for (x = 0; x < Width; x++)
        {
            MessageWindow[x][y - 1] = MessageWindow[x][y];
        }
    for (x = 0; x < Width; x++)
        MessageWindow[x][Height - 1] = ' ';
  }
  DrawMessageWindow();
}

void WriteLineMessageWindow(char message[16], byte delay)
{
    byte x;
    ScrollMessageWindowUp(1);  
    for(x = 0; x < 16; x++)
    {
      if (message[x] == '@')
      break;
      MessageWindow[x][Height-1] = message[x];
      SetScreenChar(message[x], Color, PosX + x, PosY + Height - 1);
      wait_vblank(delay);
    }
    //DrawMessageWindow();
}