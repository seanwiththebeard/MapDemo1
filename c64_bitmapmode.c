#include <c64.h>
#include <peekpoke.h>
#include "common.h"
#include "bitwiseops.h"

const int CharacterRam = 0x3000;
const int CharacterRom = 0xD000;

void setcolortextmode()
{
  int i = 0;
  
  //Copy character set
  POKE(0x0034, 48);  // Reserve RAM
  POKE(0x0038, 48); // Reserve RAM
  POKE(0xDC0E, PEEK(0xDC0E)&254); // Pause Keyscan
  POKE(0x0001, (PEEK(0x0001)&251)); // Character ROM select
  
  //Copy the character set to the new RAM location
  for(i = 0; i < 1024; i++)
  {
    POKE(i + CharacterRam, PEEK(i + CharacterRom));
  }
  
  POKE(0x0001, (PEEK(0x0001)|4)); // Character ROM de-select, back to IO
  POKE(0xDC0E, PEEK(0xDC0E)|1); // Resume Keyscan
  
  //Redirect the character set address, select uppercase set
  POKE (0xD018, (PEEK(0xD018)&240) +12);
  
  for(i = 0; i < 8; i++)
  {
    //POKE(i + 0x3000, 255);
  }
  
  //Select first character set
  //POKE(0xD018, 21);
  //Select second character set
  //POKE(0xD018, 23);
  
  //Background / 00
  POKE(0xD021, 0);
  //Foreground / 01
  POKE(0xD022, 5);
  //Foreground / 10
  POKE(0xD023, 1);
  
  //Foreground / 11 / 0-7 single, 8-15 multi
  for (i = 0; i < 1000; i++)
    POKE(0xD800 + i, 1);
}

void ScrollChar(byte index, byte direction)
{
  byte buffer[8];
  int i;
  int origin = 0x3000 + 8*index;
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