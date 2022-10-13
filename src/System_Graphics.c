#if __C64__
#include <c64.h>
#endif
#if __apple2__
#include <apple2.h>
#endif
#include <peekpoke.h>
#include "Common.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "System_CharacterSets.h"
#include "Platform.h"

byte charScrollBuffer[8], column, OffsetY, temp, count, ColCount;
int offset, charOffset, colorOffset, origin, retValue, bufferColorAddress, bufferScreenAddress, i;

byte *ScreenCharBuffer = (byte *)0x0400;
byte *ScreenColorBuffer = (byte *)0xF400;
byte *ScreenChars = (byte *)0x0400;
byte *ScreenColors = (byte *)0xD800;

byte *CharRam;
byte lastBank, lastScreenPos, lastCharPos;
unsigned short offset1;

#if __apple2__
byte* HGR = (byte*)0x2000;
byte* HGRBuffer = (byte*)0x4000;
int RowsHGR[192];
#endif

int screenposition;

int YColumnIndex[25] = {
  0, 40, 80, 120, 160,
  200, 240, 280, 320, 360,
  400, 440, 480, 520, 560,
  600, 640, 680, 720, 760,
  800, 840, 880, 920, 960};

int FlashFrames = 0;

bool bufferselect = 0;

void ScrollingMaskOn()
{
  #if __C64__
  clearBit(VIC.ctrl1, 3);
  clearBit(VIC.ctrl2, 3);
  #endif
}

void ScrollingMaskOff()
{
  #if __C64__
  setBit(VIC.ctrl1, 3);
  setBit(VIC.ctrl2, 3);
  #endif
}

void ScrollReset()
{
  #if __C64__
    VIC.ctrl1 = 0x1b;
    VIC.ctrl2 = 0xc8;
  #endif
}

void FlipBuffer()
{
  {
    if (bufferselect)
    {
      bufferselect = false;
      SelectVICBanks(lastBank, lastScreenPos - 1, lastCharPos);
    }
    else
    {
      bufferselect = true;
      SelectVICBanks(lastBank, lastScreenPos + 1, lastCharPos);
    }
  }
}

void MoveScreenUp()
{
  #if __C64__
  byte ys;
  wait_vblank(1);
  CopyMemory(&ScreenCharBuffer[0], &ScreenChars[COLS], YColumnIndex[ROWS - 1]);
  FlipBuffer();
  for (ColCount = 0; ColCount < COLS; ++ColCount)
    ScreenCharBuffer[YColumnIndex[ROWS - 1]+ColCount] = ' ';
  for (ys = 7; ys != 0; --ys)
  {
    // set scroll registers
    #if __C64__
    VIC.ctrl1 = 144 | (ys & 7);
    #endif
    wait_vblank(4);
  }
  #endif

  #if __apple2__
  gotoxy(0, 23);
  cprintf("Hello\n");
  #endif
  
}

void MoveScreenDown()
{
  byte ys;
  CopyMemory(&ScreenCharBuffer[COLS], &ScreenChars[0], YColumnIndex[24]);
  FlipBuffer();

  #if __C64__
  for (ys = 0; ys < 7; ++ys)
  {
    // set scroll registers
    VIC.ctrl1 = (VIC.ctrl1 & 0xf8);
    VIC.ctrl1 |= (ys & 7);
    VIC.ctrl1 = clearBit(VIC.ctrl1, 3);
    // wait for vsync
    wait_vblank(2);
  }
  #endif

  for (ColCount = 0; ColCount < COLS; ++ColCount)
    ScreenCharBuffer[ColCount] = ' ';
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
  #if __C64__
  offset1 = YColumnIndex[y] + x;
  ScreenChars[offset1] = index;;
  ScreenColors[offset1] = AttributeSet[index];
  #endif


  #if __apple2__
  index = (int)index <<3;
  ypos = ypos << 3;
  offset= RowsHGR[ypos] + xpos;
  for (y = 0; y < 8; ++y)
  {
    HGR[offset] = charset[index];
    offset += 0x400;
    ++index;
  }
  #endif
}

void SetCharBuffer(byte x, byte y, byte index)
{
  #if __C64__
  offset1 = YColumnIndex[y] + x;
  ScreenCharBuffer[offset1] = index;;
  ScreenColorBuffer[offset1] = AttributeSet[index];
  #endif


  #if __apple2__
  index = (int)index <<3;
  ypos = ypos << 3;
  offset= RowsHGR[ypos] + xpos;
  for (y = 0; y < 8; ++y)
  {
    HGR[offset] = charset[index];
    offset += 0x400;
    ++index;
  }
  #endif
}

void DrawCharacterSet(byte destX, byte destY)
{
  #if __C64__
  byte posX, posY;
  DrawBorder(destX - 1, destY - 1, 18, 18, false);
   for (posY = 0; posY < 16; ++posY)
        for (posX = 0; posX < 16; ++posX)
                SetChar(posY*16 + posX, posX + destX, posY + destY);
  CopyDoubleBufferArea(destX, destY, 16, 16);
  #endif
}


int originOffset = 0;

void SetTileOrigin(byte x, byte y)
{
  originOffset = YColumnIndex[y] + x;  
}

byte indexes[4];

void DrawTileFast(byte index, byte x, byte y)
{ 
  index = (index << 1) + ((index >> 3) << 4);
  indexes[0] = index;
  indexes[1] = index + 1;
  indexes[2] = index + 16;
  indexes[3] = index + 17;
  
  x = x << 1;
  y = y << 1;

  offset1 = YColumnIndex[y] + x + originOffset;
  //if (buffer)
  {
    memcpy(&ScreenCharBuffer[offset1], &indexes[0], 2);
    memcpy(&ScreenColorBuffer[offset1], &AttributeSet[indexes[0]], 2);
    offset1 += COLS;
    memcpy(&ScreenCharBuffer[offset1], &indexes[2], 2);
    memcpy(&ScreenColorBuffer[offset1], &AttributeSet[indexes[2]], 2);
  }
  /*else
  {
    memcpy(&ScreenChars[offset1], &indexes[0], 2);
    memcpy(&ScreenColors[offset1], &AttributeSet[indexes[0]], 2);
    offset1 += COLS;
    memcpy(&ScreenChars[offset1], &indexes[2], 2);
    memcpy(&ScreenColors[offset1], &AttributeSet[indexes[2]], 2);
  }*/
}

void CopyDoubleBuffer()
{
  memcpy((int*)ColorRam, &ScreenColorBuffer[0], 1000);
  memcpy(&ScreenChars[0], &ScreenCharBuffer[0], 1000);
}

void CopyDoubleBufferArea(byte posX, byte posY, byte sizeX, byte sizeY)
{
  #if __C64__
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
    #endif
}

void ClearScreen()
{
  #if __C64__
  for (i = 0; i < 1000; ++i)
  {
    ScreenColors[i] = 1;
    ScreenColorBuffer[i] = 1;
    ScreenCharBuffer[i] = ' ';
    ScreenChars[i] = ' ';
  }
  
  FlipBuffer();
  #endif

  #if __apple2__
  clrscr();
  #endif
  
}

void ReverseBufferArea(byte posX, byte posY, byte sizeX, byte sizeY)
{
  #if __C64__

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
  #endif
}

void SelectVICBanks(byte bank, byte screenpos, byte charpos)
{
  #if __C64__
  int* regd018 = (int*)0xd018;
  byte vicreg = 0x00;
  
  lastBank = bank;
  lastScreenPos = screenpos;
  lastCharPos = charpos;

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
  raster_wait(252);
  regd018[0] = vicreg;
  //POKE (0xD018, screenpos + charpos);
 
  
  //Cursor Position
  POKE (0x0288, screenposition / 256);
  #endif

  #if __apple2__
  byte y;
  #define STROBE(addr)       __asm__ ("sta %w", addr)
  STROBE(0xc052); // turn off mixed-mode
  STROBE(0xc054); // page 1
  STROBE(0xc057); // hi-res
  STROBE(0xc050); // set graphics mode
  //memset((byte*)0x2000, 0, 0x2000); // clear page 1
  for (y = 0; y < 192; ++y)
    RowsHGR[y] = (y/64)*0x28 + (y%8)*0x400 + ((y/8)&7)*0x80;
  #endif
}

void DrawLineH(char index, byte x, byte y, byte length)
{
  for (count = 0; count < length; ++count)
    SetCharBuffer(x + count, y, index);
}

void DrawLineV(char index, byte x, byte y, byte length)
{
  for (count = 0; count < length; ++count)
    SetCharBuffer(x, y + count, index);
}

void PrintString(char text[20], byte posx, byte posy, bool fast, bool buffer)
{
  #if __C64__
  for(count = 0; count < 20; ++count)
  {
    if (text[count] == '@')
      break;
    if (!fast)
      raster_wait(255);
    if (buffer)
      SetCharBuffer(posx + count, posy, text[count]);
    else
      SetChar(posx + count, posy, text[count]);      
  }
  #endif

  #if __apple2__
  gotoxy(posx, posy);
  cprintf(text)
  #endif
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

void DrawBorder(byte xPos, byte yPos, byte width, byte height, bool fill)
{
  byte x;
  
  DrawLineH(239, xPos + 1, yPos, width - 2);
  DrawLineH(239, xPos + 1, yPos + height - 1, width - 1);
  DrawLineV(255, xPos, yPos + 1, height - 1);
  DrawLineV(255, xPos + width - 1, yPos + 1, height - 1);
  SetCharBuffer(xPos, yPos, 238);
  SetCharBuffer(xPos + width - 1, yPos, 238);
  SetCharBuffer(xPos, yPos + height - 1, 238);
  SetCharBuffer(xPos + width - 1, yPos + height - 1, 238);
  if (fill)
    for (x = 0; x < height - 2; ++x)
    {
      DrawLineH(' ', xPos + 1, yPos + x + 1, width - 2);
    }
}