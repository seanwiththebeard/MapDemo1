#include <c64.h>
#include <peekpoke.h>
#include "Common.h"
#include <conio.h>
#include <stdio.h>
#include "System_CharacterSets.h"
#include "Platform.h"

byte charScrollBuffer[8], column, OffsetY, temp, count, ColCount;
int offset, charOffset, colorOffset, origin, retValue, bufferColorAddress, bufferScreenAddress, i;

byte *ScreenCharBuffer = 0;
byte *ScreenColorBuffer = 0;

byte *ScreenChars;
byte *ScreenColors = (byte *)0xD800;
byte *CharRam;
unsigned short offset1;

int screenposition;

int YColumnIndex[25] = {
  0, 40, 80, 120, 160,
  200, 240, 280, 320, 360,
  400, 440, 480, 520, 560,
  600, 640, 680, 720, 760,
  800, 840, 880, 920, 960};

int FlashFrames = 0;

bool bufferselect = 0;
byte charpos = 2;

void FlipBuffer()
{
  {
    if (bufferselect)
    {
      SelectVICBanks(3, charpos + 1, 7);
      bufferselect = false;
    }
    else
    {
      SelectVICBanks(3, charpos, 7);
      bufferselect = true;
    }
  }
}
void MoveScreenUp()
{
  byte ys = 0;
  CopyMemory(&ScreenCharBuffer[0], &ScreenChars[COLS], YColumnIndex[24]);
  FlipBuffer();
  
  for (ys = 7; ys > 0; ys--)
  {
    // set scroll registers
    VIC.ctrl1 = (VIC.ctrl1 & 0xf8);
    VIC.ctrl1 |= (ys & 7);
    // wait for vsync
    wait_vblank(1);
  }
  
  for (ColCount = 0; ColCount < COLS; ++ColCount)
    ScreenCharBuffer[960+ColCount] = ' ';
  
  //wait_vblank(1);

}

void UpdateColors()
{
  for (offset = 0; offset < 1000; ++offset)
  {
    ScreenColors[offset] = AttributeSet[PEEK(&ScreenChars[0] + offset)];
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

int originOffset = 0;

void SetTileOrigin(byte x, byte y)
{
  originOffset = YColumnIndex[y] + x;  
}

byte indexes[4];

void DrawTileFast(byte index, byte x, byte y, bool buffer)
{ 
  index = (index << 1) + ((index >> 3) << 4);
  indexes[0] = index;
  indexes[1] = index + 1;
  indexes[2] = index + 16;
  indexes[3] = index + 17;
  
  x = x << 1;
  y = y << 1;

  offset1 = YColumnIndex[y] + x + originOffset;
  if (buffer)
  {
    memcpy(&ScreenCharBuffer[offset1], &indexes[0], 2);
    memcpy(&ScreenColorBuffer[offset1], &AttributeSet[indexes[0]], 2);
    offset1 += COLS;
    memcpy(&ScreenCharBuffer[offset1], &indexes[2], 2);
    memcpy(&ScreenColorBuffer[offset1], &AttributeSet[indexes[2]], 2);
  }
  else
  {
    memcpy(&ScreenChars[offset1], &indexes[0], 2);
    memcpy(&ScreenColors[offset1], &AttributeSet[indexes[0]], 2);
    offset1 += COLS;
    memcpy(&ScreenChars[offset1], &indexes[2], 2);
    memcpy(&ScreenColors[offset1], &AttributeSet[indexes[2]], 2);
  }
}

void CopyDoubleBuffer()
{
  memcpy((int*)ColorRam, &ScreenColorBuffer[0], 1000);
  memcpy(&ScreenChars[0], &ScreenCharBuffer[0], 1000);
  
}

void CopyDoubleBufferArea(byte posX, byte posY, byte sizeX, byte sizeY)
{
  offset = posX + YColumnIndex[posY];
  OffsetY = posX + YColumnIndex[0];
  charOffset = (int)&ScreenChars[0] + offset;
  colorOffset = ColorRam + offset;
  bufferScreenAddress = (int)&ScreenCharBuffer[offset];
  bufferColorAddress = (int)&ScreenColorBuffer[offset];
  
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
  for (i = 0; i < 1000; ++i)
  {
    ScreenColors[i] = 1;
    ScreenColorBuffer[i] = 1;
    ScreenCharBuffer[i] = ' ';
    ScreenChars[i] = ' ';
  }
  
  FlipBuffer();
  
  
}

void ReverseBufferArea(byte posX, byte posY, byte sizeX, byte sizeY)
{
  offset = posX + YColumnIndex[posY];
  OffsetY = posX + YColumnIndex[0];
  bufferScreenAddress = (int)&ScreenChars[0] + offset;
  bufferColorAddress = ColorRam + offset;
  charOffset = (int)&ScreenCharBuffer[offset];
  colorOffset = (int)&ScreenColorBuffer[offset];
  
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

void SelectVICBanks(byte bank, byte screenpos, byte charpos)
{
  int* regd018 = (int*)0xd018;
  byte vicreg = 0x00;
  
  
  screenposition = (bank * (16*1024) + (screenpos * 1024));
  
  ScreenChars = 0;
  ScreenChars += screenposition;
  
  CharRam = 2;
  CharRam += (bank * (16*1024) + charpos * 2048);
  
  ScreenCharBuffer = 0;
  ScreenCharBuffer += screenposition;
  if (bufferselect)
    ScreenCharBuffer -= 0x0400; // Buffer location 1024b after the screen position
  else
    ScreenCharBuffer += 0x0400; // Buffer location 1024b after the screen position
    
  
  ScreenColorBuffer = 0;
  ScreenColorBuffer += 0x0400; // Use the default screen character space for color buffer
  
  /*
  Bank 3:
  character generator ROM is unavailable – Must be copied to RAM
  4K of RAM – unused by the system
  4K of I/O registers
  8K Operating System Kernal ROM
  good place for graphics memory
  BASIC is not limited
  contains enough free RAM for 4 text screens
  ROM area could be used to store two character sets and 64 sprite shapes
  No RAM exits for a high resolution screen
  Avoid using 52224-53247 for graphics if using DOS support
  Hi-res could be acccessed by using the area under Kernal ROM – need to turn off interrupts and switch out the ROM to use the RAM

  $DD00 = %xxxxxx11 -> bank0: $0000-$3fff
  $DD00 = %xxxxxx10 -> bank1: $4000-$7fff
  $DD00 = %xxxxxx01 -> bank2: $8000-$bfff
  $DD00 = %xxxxxx00 -> bank3: $c000-$ffff*/
  
   //Select Bank
  POKE (0xDD00, (PEEK(0XDD00)&(255 - bank)));
  
  //Set Screen and Character Ram Position
  screenpos = screenpos << 4;
  charpos = charpos << 1;
  vicreg = screenpos + charpos;
  raster_wait(255);
  regd018[0] = vicreg;
  //POKE (0xD018, screenpos + charpos);
 
  
  //Cursor Position
  POKE (0x0288, screenposition / 256);
}

/*void setcolortextmode()
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
}*/

void SetScreenChar(byte index, byte xpos, byte ypos)
{
  offset = YColumnIndex[ypos] + xpos;
  POKE(&ScreenCharBuffer[offset], index);
  POKE(&ScreenColorBuffer[offset], AttributeSet[index]);
}

void SetScreenCharColor(byte index, byte color, byte xpos, byte ypos)
{  
  offset = YColumnIndex[ypos] + xpos;
  POKE(&ScreenCharBuffer[offset], index);
  POKE(&ScreenColorBuffer[offset], color);
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

void PrintString(char text[20], byte posx, byte posy, bool fast, bool buffer)
{
  for(count = 0; count < 20; ++count)
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
  origin = (int)&CharRam[0] + 8*index;
  
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
