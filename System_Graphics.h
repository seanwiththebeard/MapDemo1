#include "common.h"

void setcolortextmode();
void SetScreenChar(byte index, byte color, byte xpos, byte ypos);
void ScrollChar(byte index, byte direction);
void FlashColor(byte index, byte length);
void FlashColorWait(byte index, byte length);
void Graphics_Update();
void PrintString(char text[16], byte posx, byte posy, bool fast);
void DrawLineH(char index, byte color, byte x, byte y, byte length);
void DrawLineV(char index, byte color, byte x, byte y, byte length);