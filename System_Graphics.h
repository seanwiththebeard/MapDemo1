#include "Common.h"

void SetChar(byte x, byte y, byte index);
void SetCharC(byte x, byte y, byte index, byte color);

void UpdateColors(void);
void ClearScreen(void);

void CopyDoubleBuffer(void);
//void CopyDoubleBufferRows(byte posY, byte sizeY, byte length);
void CopyDoubleBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);
void ReverseBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);
void setcolortextmode(void);
void SetScreenChar(byte index, byte xpos, byte ypos);
void SetScreenCharColor(byte index, byte color, byte xpos, byte ypos);
void ScrollChar(byte index, byte direction);
void FlashColor(byte index, byte length);
void FlashColorWait(byte index, byte length);
void Graphics_Update(void);
void PrintString(char text[16], byte posx, byte posy, bool fast, bool buffer);
void DrawLineH(char index, byte color, byte x, byte y, byte length);
void DrawLineV(char index, byte color, byte x, byte y, byte length);
void DrawBorder(byte xPos, byte yPos, byte width, byte height, bool buffer);

extern int YColumnIndex[];
extern byte ScreenDoubleBuffer[2000];