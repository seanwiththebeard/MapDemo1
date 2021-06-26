#include <c64.h>
#include <peekpoke.h>
#include "common.h"
#include <conio.h>
#include <stdio.h>
//#include "cbm_petscii_charmap.h"
#include "System_CharacterSets.h"

const int CharacterRam = 0xC000;
const int CharacterRom = 0xD000;

int FlashFrames = 0;

void setcolortextmode()
{
  int i = 0;
  //Copy character set
  //POKE(0x0034, 48);  // Reserve RAM
  //POKE(0x0038, 48); // Reserve RAM
  POKE(0xDC0E, PEEK(0xDC0E)&254); // Pause Keyscan
  POKE(0x0001, (PEEK(0x0001)&251)); // Character ROM select
  
  //Copy the character set to the new RAM location
  //for(i = 0; i < 2048; i++)
    //POKE(i + CharacterRam, PEEK(i + CharacterRom));
  
  POKE(0x0001, (PEEK(0x0001)|4)); // Character ROM de-select, back to IO
  POKE(0xDC0E, PEEK(0xDC0E)|1); // Resume Keyscan
  
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
  
  //Background / 00
  POKE(0xD021, 0);
  //Foreground / 01
  POKE(0xD022, 5);
  //Foreground / 10
  POKE(0xD023, 1);
  
  //Foreground / 11 / 0-7 single, 8-15 multi
  for (i = 0; i < 1024; i++)
  {
    //POKE(ColorRam + i, 1);
  }
}

void SetScreenChar(byte index, byte color, byte xpos, byte ypos)
{  
  int offset = 40*ypos + xpos;
  POKE(0xC800 + offset, index);
  POKE(0xD800 + offset, color);
}

void DrawLineH(char index, byte color, byte x, byte y, byte length)
{
  int z;
  for (z = 0; z < length; z++)
    SetScreenChar(index, color, x + z, y);
}

void DrawLineV(char index, byte color, byte x, byte y, byte length)
{
  int z;
  for (z = 0; z < length; z++)
    SetScreenChar(index, color, x, y + z);
}

void PrintString(char text[16], byte posx, byte posy, bool fast)
{
  int i;
  for(i = 0; i < 16; i++)
  {
    if (!fast)
      raster_wait(255);
    SetScreenChar(text[i], AttributeSet[0][text[i]], posx + i, posy);
  }
}

void ScrollChar(byte index, byte direction)
{
  byte buffer[8];
  int i;
  int origin = CharacterRam + 8*index;
  byte temp;
  
  for(i = 0; i < 8; i++)
    buffer[i] = PEEK(origin+i);
  
  switch (direction)
  {
    case 0:  //Scroll Down
      for(i = 1; i < 8; i++)
        POKE(i + origin, buffer[i-1]);
      POKE(origin, buffer[7]);
      break;
    case 1: // Scroll Up
      for(i = 0; i < 8; i++)
        POKE(i + origin, buffer[i+1]);
      POKE(origin + 7, buffer[0]);
      break;
    case 2: //  Scroll Right
      for(i = 0; i < 8; i++)
      {
        temp = buffer[i] >> 1;
        //temp >> 1;
        WriteBit(&temp, 7, ReadBit(buffer[i], 0));
        POKE(origin + i, temp);
      }
      break;
    case 3: // Scroll Left
      for(i = 0; i < 8; i++)
      {
        temp = buffer[i] << 1;
        //temp >> 1;
        WriteBit(&temp, 0, ReadBit(buffer[i], 7));
        POKE(origin + i, temp);
      }
      break;
  }
}

void FlashColorWait(byte index, byte length)
{
  int i = 0;
  int retValue = PEEK(0xD021);
  POKE(0xD021, index);
  for (i = 0; i < length; i++)
    raster_wait(255);
  POKE(0xD021, retValue);   
    
  //POKE(0xD021, index);
}

void FlashColor(byte index, byte length)
{
  POKE(0xD021, index);
  FlashFrames = length;
}

void Graphics_Update()
{
  if (FlashFrames > 0)
  {
    FlashFrames--;
    if (FlashFrames == 0)
    POKE(0xD021, 0);

  }
}