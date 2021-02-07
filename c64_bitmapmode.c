#include <c64.h>
#include <peekpoke.h>
#include "common.h"
#include "bitwiseops.h"

const int CharacterRam = 0xC000;
const int CharacterRom = 0xD000;
const int ColorRam = 0xD800;

void setcolortextmode()
{
  int i = 0;
  
  
  //Copy character set
  POKE(0x0034, 48);  // Reserve RAM
  POKE(0x0038, 48); // Reserve RAM
  POKE(0xDC0E, PEEK(0xDC0E)&254); // Pause Keyscan
  POKE(0x0001, (PEEK(0x0001)&251)); // Character ROM select
  
  //Copy the character set to the new RAM location
  for(i = 0; i < 2048; i++)
  {
    POKE(i + CharacterRam, PEEK(i + CharacterRom));
  }
  
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
    POKE(ColorRam + i, 1);
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