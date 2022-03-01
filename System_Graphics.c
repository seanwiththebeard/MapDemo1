#include <c64.h>
#include <peekpoke.h>
#include "Common.h"
#include <conio.h>
#include <stdio.h>
#include "System_CharacterSets.h"
#include "System_Graphics.h"

byte ScreenDoubleBuffer[2000], charScrollBuffer[8], column, OffsetY, temp, count;
int offset, charOffset, colorOffset, origin, retValue, bufferColorAddress, bufferScreenAddress;

int YColumnIndex[25] = {
  0, 40, 80, 120, 160,
  200, 240, 280, 320, 360,
  400, 440, 480, 520, 560,
  600, 640, 680, 720, 760,
  800, 840, 880, 920, 960};

int FlashFrames = 0;

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
  //POKE (0x0288, 200);
  
  for (offset = 0; offset < 1000; ++offset)
  {
    ScreenDoubleBuffer[offset] = ' ';
    ScreenDoubleBuffer[offset + 1000] = ' ';
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
    SetScreenCharColor(index, color, x + count, y);
}

void DrawLineV(char index, byte color, byte x, byte y, byte length)
{
  for (count = 0; count < length; ++count)
    SetScreenCharColor(index, color, x, y + count);
}

void PrintString(char text[16], byte posx, byte posy, bool fast)
{
  for(count = 0; count < 16; ++count)
  {
    if (!fast)
      raster_wait(255);
    SetScreenChar(text[count], posx + count, posy);
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