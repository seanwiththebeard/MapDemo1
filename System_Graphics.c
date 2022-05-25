#include <c64.h>
#include <peekpoke.h>
#include "Common.h"
#include <conio.h>
#include <stdio.h>
#include "System_CharacterSets.h"
#include "System_Graphics.h"

byte ScreenDoubleBuffer[2000], charScrollBuffer[8], column, OffsetY, temp, count;
int offset, charOffset, colorOffset, origin, retValue, bufferColorAddress, bufferScreenAddress, i;

byte far *ScreenChars = (byte far*)ScreenRam;
byte far *ScreenColors = (byte far*)ColorRam;    
unsigned short offset1;


int YColumnIndex[25] = {
  0, 40, 80, 120, 160,
  200, 240, 280, 320, 360,
  400, 440, 480, 520, 560,
  600, 640, 680, 720, 760,
  800, 840, 880, 920, 960};

int FlashFrames = 0;

void UpdateColors()
{
  for (offset = 0; offset < 1000; ++offset)
  {
    ScreenColors[offset] = AttributeSet[PEEK(ScreenRam + offset)];
  }
}


void SetChar(byte x, byte y, byte index)
{
  offset1 = YColumnIndex[y] + x;
  ScreenChars[offset1] = index;;
  ScreenColors[offset1] = AttributeSet[index];
}

void SetCharC(byte x, byte y, byte index, byte color)
{
  offset1 = YColumnIndex[y] + x;
  ScreenChars[offset1] = index;;
  ScreenColors[offset1] = color;
}

void DrawCharacterSet(byte destX, byte destY)
{
  byte posX, posY;
  DrawBorder(destX - 1, destY - 1, 18, 18, false, false);
   for (posY = 0; posY < 16; ++posY)
        for (posX = 0; posX < 16; ++posX)
                SetScreenChar(posY*16 + posX, posX + destX, posY + destY);
  CopyDoubleBufferArea(destX, destY, 16, 16);

}

void CopyDoubleBuffer()
{
  CopyMemory(ColorRam, (int)&ScreenDoubleBuffer[1000], 1000);
  CopyMemory(ScreenRam, (int)&ScreenDoubleBuffer[0], 1000);
}

void CopyDoubleBufferArea(byte posX, byte posY, byte sizeX, byte sizeY)
{
  offset = posX + YColumnIndex[posY];
  OffsetY = posX + YColumnIndex[0];
  charOffset = ScreenRam + offset;
  colorOffset = ColorRam + offset;
  bufferScreenAddress = (int)&ScreenDoubleBuffer[offset];
  bufferColorAddress = (int)&ScreenDoubleBuffer[offset + 1000];
  
  raster_wait(240);
  for (column = 0; column < sizeY; ++column)
    {
    	if (column % 6 == 0)
          raster_wait(240);
      	CopyMemory(charOffset, bufferScreenAddress, sizeX);
      	CopyMemory(colorOffset, bufferColorAddress, sizeX);
      	charOffset += COLS;
      	colorOffset += COLS;
      	OffsetY += COLS;
    	bufferScreenAddress += COLS;
    	bufferColorAddress += COLS;
    } 
}

void ClearScreen()
{
  for (i = 1000; i < 2000; ++i)
  {
    ScreenDoubleBuffer[i] = 0;
  }
  for (i = 0; i < 1000; ++i)
  {
    ScreenDoubleBuffer[i] = ' ';
  }
  
  CopyDoubleBuffer();
}

void ReverseBufferArea(byte posX, byte posY, byte sizeX, byte sizeY)
{
  offset = posX + YColumnIndex[posY];
  OffsetY = posX + YColumnIndex[0];
  bufferScreenAddress = ScreenRam + offset;
  bufferColorAddress = ColorRam + offset;
  charOffset = (int)&ScreenDoubleBuffer[offset];
  colorOffset = (int)&ScreenDoubleBuffer[offset + 1000];
  
  raster_wait(240);
  for (column = 0; column < sizeY; ++column)
    {
    	if (column % 6 == 0)
          raster_wait(240);
      	CopyMemory(charOffset, bufferScreenAddress, sizeX);
      	CopyMemory(colorOffset, bufferColorAddress, sizeX);
      	charOffset += COLS;
      	colorOffset += COLS;
      	OffsetY += COLS;
    	bufferScreenAddress += COLS;
    	bufferColorAddress += COLS;
    }
  
}

void setcolortextmode()
{
  //POKE(0xDC0E, PEEK(0xDC0E)&254); // Pause Keyscan
  //POKE(0x0001, (PEEK(0x0001)&251)); // Character ROM select
  
  //POKE(0x0001, (PEEK(0x0001)|4)); // Character ROM de-select, back to IO
  //POKE(0xDC0E, PEEK(0xDC0E)|1); // Resume Keyscan
  
  //Redirect the character set address, select uppercase set
  //Select Bank 3 (last bank)
  POKE (0xDD00, (PEEK(0XDD00)&252));
  
  //Set character RAM + screen memory (?) position
  //Numbers here are RELATIVE TO THE BANK ADDRESS
  //The four most significant bits form a 4-bit number
  //in the range 0 thru 15: Multiplied with 1024 this gives the start address 
  //for the screen character RAM.
  //Bits 1 thru 3 (weights 2 thru 8) form a 3-bit number
  //in the range 0 thru 7: Multiplied with 2048 this gives the start address 
  //for the character set.
  //
  //We want to use the start of the bank (0) for CHRSET and add 2KB for the screen RAM
  // 0010 000X
  POKE (0xD018, 32);
  
  // Set Kernel-Function Screen Position pointer (used for ClrScr, Printf)
  // Value * 256 = Screen Position Address
  // 1100 1000
  POKE (0x0288, 200);
  
  for (offset = 0; offset < 1000; ++offset)
  {
    ScreenDoubleBuffer[offset] = ' ';
    ScreenDoubleBuffer[offset + 1000] = 0;
  }
  CopyDoubleBuffer();
}

void SetScreenChar(byte index, byte xpos, byte ypos)
{  
  offset = YColumnIndex[ypos] + xpos;
  POKE(&ScreenDoubleBuffer[offset], index);
  POKE(&ScreenDoubleBuffer[offset + 1000], AttributeSet[index]);
}

void SetScreenCharColor(byte index, byte color, byte xpos, byte ypos)
{  
  offset = YColumnIndex[ypos] + xpos;
  POKE(&ScreenDoubleBuffer[offset], index);
  POKE(&ScreenDoubleBuffer[offset + 1000], color);
}

void DrawLineH(char index, byte color, byte x, byte y, byte length)
{
  for (count = 0; count < length; ++count)
    SetCharC(x + count, y, index, color);
    //SetScreenCharColor(index, color, x + count, y);
}

void DrawLineV(char index, byte color, byte x, byte y, byte length)
{
  for (count = 0; count < length; ++count)
    SetCharC(x, y + count, index, color);
    //SetScreenCharColor(index, color, x, y + count);
}

void PrintString(char text[16], byte posx, byte posy, bool fast, bool buffer)
{
  for(count = 0; count < 16; ++count)
  {
    if (text[count] == '@')
      break;
    if (!fast)
      raster_wait(255);
    if (buffer)
      SetScreenChar(text[count], posx + count, posy);
    else
      SetChar(posx + count, posy, text[count]);      
  }
}

void ScrollChar(byte index, byte direction)
{
  origin = CharacterRam + 8*index;
  
  for(count = 0; count < 8; ++count)
    charScrollBuffer[count] = PEEK(origin+count);
  
  switch (direction)
  {
    case 0:  //Scroll Down
      for(count = 1; count < 8; ++count)
        POKE(count + origin, charScrollBuffer[count-1]);
      POKE(origin, charScrollBuffer[7]);
      break;
    case 1: // Scroll Up
      for(count = 0; count < 8; ++count)
        POKE(count + origin, charScrollBuffer[count+1]);
      POKE(origin + 7, charScrollBuffer[0]);
      break;
    case 2: //  Scroll Right
      for(count = 0; count < 8; ++count)
      {
        temp = charScrollBuffer[count] >> 1;
        //temp >> 1;
        WriteBit(&temp, 7, ReadBit(charScrollBuffer[count], 0));
        POKE(origin + count, temp);
      }
      break;
    case 3: // Scroll Left
      for(count = 0; count < 8; count++)
      {
        temp = charScrollBuffer[count] << 1;
        //temp >> 1;
        WriteBit(&temp, 0, ReadBit(charScrollBuffer[count], 7));
        POKE(origin + count, temp);
      }
      break;
  }
}

void DrawBorder(byte xPos, byte yPos, byte width, byte height, bool buffer, bool fill)
{
  byte x;
  #define lineColor 15
  #define cornerColor 8
  
  DrawLineH(239, lineColor, xPos + 1, yPos, width - 2);
  DrawLineH(239, lineColor, xPos + 1, yPos + height - 1, width - 1);
  DrawLineV(255, lineColor, xPos, yPos + 1, height - 1);
  DrawLineV(255, lineColor, xPos + width - 1, yPos + 1, height - 1);
  SetCharC(xPos, yPos, 238, cornerColor);
  SetCharC(xPos + width - 1, yPos, 238, cornerColor);
  SetCharC(xPos, yPos + height - 1, 238, cornerColor);
  SetCharC(xPos + width - 1, yPos + height - 1, 238, cornerColor);
  if (fill)
    for (x = 0; x < height - 2; ++x)
    {
      DrawLineH(' ', 0, xPos + 1, yPos + x + 1, width - 2);
    }
  if (buffer)
  {
    ReverseBufferArea(xPos, yPos, width, height);
  }
}

void FlashColorWait(byte index, byte length)
{
  retValue = PEEK(0xD021);
  bgcolor(index);
  bordercolor(index);
  wait_vblank(length);
  bgcolor(retValue);
  bordercolor(retValue);
}

void FlashColor(byte index, byte length)
{
  bgcolor(index);
  bordercolor(index);
  FlashFrames = length;
}

void Graphics_Update()
{
  if (FlashFrames > 0)
  {
    FlashFrames--;
    if (FlashFrames == 0)
    bgcolor(0);
    bordercolor(0);

  }
}